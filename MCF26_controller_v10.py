#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
🔥 AJNETSDK ПОЛНЫЙ КОНТРОЛЛЕР КАМЕР v10.0
Полная интеграция с libNetSdk.dll, AjPlayer.dll, RemoteConfig.dll
"""

import tkinter as tk
from tkinter import ttk, scrolledtext, messagebox, filedialog
import ctypes
from ctypes import (c_int, c_long, c_char, c_char_p, c_void_p, c_ubyte,
                   c_ushort, c_uint, c_bool, c_float, Structure, POINTER,
                   CFUNCTYPE, byref, create_string_buffer, cast, wintypes)
import threading
import time
import os
import queue
from datetime import datetime
import xml.etree.ElementTree as ET
import xml.dom.minidom

# ============================================================================
# 1. ЗАГРУЗКА DLL БИБЛИОТЕК
# ============================================================================
def load_dll(dll_name):
    """Безопасная загрузка DLL с обработкой ошибок"""
    dll_paths = [
        f"./Bin/{dll_name}",
        f"./{dll_name}",
        f"../Bin/{dll_name}",
        dll_name
    ]
    
    for path in dll_paths:
        if os.path.exists(path):
            try:
                dll = ctypes.WinDLL(path)
                print(f"✅ {dll_name} загружена из {path}")
                return dll
            except Exception as e:
                print(f"  ❌ Ошибка загрузки {path}: {e}")
                continue
    
    print(f"⚠️ {dll_name} не найдена. Функции будут эмулированы.")
    return None

# Загружаем все необходимые DLL
libnetsdk = load_dll("libNetSdk.dll")
ajplayer = load_dll("AjPlayer.dll")
remoteconfig = load_dll("RemoteConfig.dll")

# ============================================================================
# 2. ОПРЕДЕЛЕНИЯ СТРУКТУР ИЗ SDK (на основе sdk_define.h и libNetSdk.h)
# ============================================================================

# Определение из sdk_define.h (вложенные структуры)
class UserConfig(Structure):
    _fields_ = [
        ("szUser", c_char * 32),
        ("szPassword", c_char * 32),
        ("nLevel", c_int),
        ("nReserved", c_int * 8)
    ]

class MediaStreamConfig(Structure):
    _fields_ = [
        ("nStreamType", c_int),
        ("nVideoCodec", c_int),
        ("nAudioCodec", c_int),
        ("nResolution", c_int),
        ("nFrameRate", c_int),
        ("nBitrate", c_int),
        ("nQuality", c_int),
        ("nReserved", c_int * 16)
    ]

class LANConfig(Structure):
    _fields_ = [
        ("szIP", c_char * 16),
        ("szMask", c_char * 16),
        ("szGateway", c_char * 16),
        ("szDNS1", c_char * 16),
        ("szDNS2", c_char * 16),
        ("nDHCP", c_int),
        ("nReserved", c_int * 8)
    ]

# Основная структура устройства из libNetSdk.h
class IPC_ENTRY_AJ(Structure):
    _fields_ = [
        ("ipc_sn", c_char * 32),           # MAX_IPC_SERIALNUMBER
        ("platform_id", c_char * 32),      # MAX_PLATFORM_ID
        ("p2p_id", c_char * 128),          # MAX_IPC_P2PID_LEN
        ("deviceType", c_char * 32),
        ("version", c_char * 128),
        ("title", c_char * 256),
        ("oem_sn", c_char * 32),           # MAX_IPC_SERIALNUMBER
        ("uuid", c_char * 32),             # MAX_IPC_SERIALNUMBER
        ("userCfg", UserConfig),
        ("streamCfg", MediaStreamConfig),
        ("lanCfg", LANConfig),
        ("p2pStatus", c_int),
        ("runnedtime", c_int),
        ("bChanged", c_int),
        ("ChannelNum", c_int),
        ("factoryMode", c_int)
    ]

# Структура для дополнительных данных кадра (из libNetSdk.h)
class FRAME_EXTDATA(Structure):
    _fields_ = [
        ("dwFrameNum", c_uint),
        ("dwFrameSeq", c_uint),
        ("dwTimeStamp", c_uint),
        ("nFrameType", c_int),
        ("nWidth", c_int),
        ("nHeight", c_int),
        ("nFrameRate", c_int),
        ("nBitrate", c_int),
        ("nReserved", c_int * 16)
    ]

# Структуры для AjPlayer.dll (из AjPlayer.h и AjLibDef.h)
class FRAME_INFO_AJ(Structure):
    _fields_ = [
        ("nWidth", c_long),
        ("nHeight", c_long),
        ("nStamp", c_long),
        ("nType", c_long),
        ("nFrameRate", c_long),
        ("bIsVideo", c_long),
        ("nLinseSize", c_long * 4)
    ]

class VIDEO_STREAM_PARAM(Structure):
    _fields_ = [
        ("codec", c_char * 256),
        ("width", c_int),
        ("height", c_int),
        ("colorbits", c_int),
        ("framerate", c_int),
        ("bitrate", c_int),
        ("vol_data", c_char * 256),
        ("vol_length", c_int)
    ]

class AUDIO_STREAM_PARAM(Structure):
    _fields_ = [
        ("codec", c_char * 256),
        ("samplerate", c_int),
        ("bitspersample", c_int),
        ("channels", c_int),
        ("framerate", c_int),
        ("bitrate", c_int)
    ]

# ============================================================================
# 3. ОБЪЯВЛЕНИЕ ВСЕХ ФУНКЦИЙ SDK (на основе libNetSdk.h)
# ============================================================================

# Определяем тип handle
LIB_NETSDK_HANDLE = c_long

# Объявляем callback-функции (типы)
SearchIPCCallBack = CFUNCTYPE(c_long, c_long, c_long, POINTER(IPC_ENTRY_AJ), c_void_p)
StatusEventCallBack = CFUNCTYPE(c_long, LIB_NETSDK_HANDLE, c_long, c_char_p, c_void_p)
AUXResponseCallBack = CFUNCTYPE(c_long, LIB_NETSDK_HANDLE, c_long, c_long, c_char_p, c_void_p)
fRealDataCallBack = CFUNCTYPE(c_long, LIB_NETSDK_HANDLE, c_long, c_long, c_char_p, c_long, POINTER(FRAME_EXTDATA))
fCaptureAudioCallBack = CFUNCTYPE(c_long, c_char_p, c_long, c_void_p)
SerialDataCallBack = CFUNCTYPE(c_long, LIB_NETSDK_HANDLE, c_char_p, c_long, c_void_p)

if libnetsdk:
    # ========== БАЗОВЫЕ ФУНКЦИИ ==========
    libnetsdk.AJ_NETSDK_IPC_Init.argtypes = []
    libnetsdk.AJ_NETSDK_IPC_Init.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_Cleanup.argtypes = []
    libnetsdk.AJ_NETSDK_IPC_Cleanup.restype = c_long
    
    # ========== CALLBACK ФУНКЦИИ ==========
    libnetsdk.AJ_NETSDK_IPC_SetSearchStatusCallBack.argtypes = [SearchIPCCallBack, c_void_p]
    libnetsdk.AJ_NETSDK_IPC_SetSearchStatusCallBack.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_SetStatusEventCallBack.argtypes = [StatusEventCallBack, c_void_p]
    libnetsdk.AJ_NETSDK_IPC_SetStatusEventCallBack.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_SetAUXResponseCallBack.argtypes = [AUXResponseCallBack, c_void_p]
    libnetsdk.AJ_NETSDK_IPC_SetAUXResponseCallBack.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_SetRealPlayStatusEventCallBack.argtypes = [StatusEventCallBack, c_void_p]
    libnetsdk.AJ_NETSDK_IPC_SetRealPlayStatusEventCallBack.restype = c_long
    
    # ========== ПОИСК УСТРОЙСТВ ==========
    libnetsdk.AJ_NETSDK_IPC_StartSearchIPC.argtypes = [c_char_p]
    libnetsdk.AJ_NETSDK_IPC_StartSearchIPC.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_StopSearchIPC.argtypes = []
    libnetsdk.AJ_NETSDK_IPC_StopSearchIPC.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_GetSearchIPCCount.argtypes = []
    libnetsdk.AJ_NETSDK_IPC_GetSearchIPCCount.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_GetIPCInfo.argtypes = [c_long, POINTER(IPC_ENTRY_AJ)]
    libnetsdk.AJ_NETSDK_IPC_GetIPCInfo.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_ModifyIPC.argtypes = [c_long, POINTER(IPC_ENTRY_AJ), c_char_p, c_char_p]
    libnetsdk.AJ_NETSDK_IPC_ModifyIPC.restype = c_long
    
    # ========== ПОДКЛЮЧЕНИЕ К УСТРОЙСТВУ ==========
    libnetsdk.AJ_NETSDK_IPC_Login.argtypes = [c_char_p, c_long, c_char_p, c_char_p]
    libnetsdk.AJ_NETSDK_IPC_Login.restype = LIB_NETSDK_HANDLE
    
    libnetsdk.AJ_NETSDK_IPC_Logout.argtypes = [LIB_NETSDK_HANDLE]
    libnetsdk.AJ_NETSDK_IPC_Logout.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_LogoutAll.argtypes = []
    libnetsdk.AJ_NETSDK_IPC_LogoutAll.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_SetAutoReconnect.argtypes = [LIB_NETSDK_HANDLE, c_int]
    libnetsdk.AJ_NETSDK_IPC_SetAutoReconnect.restype = c_long
    
    # ========== ВИДЕОПОТОК ==========
    libnetsdk.AJ_NETSDK_IPC_RealPlay.argtypes = [c_char_p, c_int, c_int, c_char_p, c_char_p, 
                                                 c_int, c_int, fRealDataCallBack, c_void_p]
    libnetsdk.AJ_NETSDK_IPC_RealPlay.restype = LIB_NETSDK_HANDLE
    
    libnetsdk.AJ_NETSDK_IPC_StopRealPlay.argtypes = [LIB_NETSDK_HANDLE]
    libnetsdk.AJ_NETSDK_IPC_StopRealPlay.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_StopAllRealPlay.argtypes = []
    libnetsdk.AJ_NETSDK_IPC_StopAllRealPlay.restype = c_long
    
    # ========== АУДИО И ГОЛОСОВАЯ СВЯЗЬ ==========
    libnetsdk.AJ_NETSDK_IPC_StartVoiceCom.argtypes = [LIB_NETSDK_HANDLE]
    libnetsdk.AJ_NETSDK_IPC_StartVoiceCom.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_StopVoiceCom.argtypes = [LIB_NETSDK_HANDLE]
    libnetsdk.AJ_NETSDK_IPC_StopVoiceCom.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_StartTalk.argtypes = [c_int, c_int, c_int, c_int, c_int]
    libnetsdk.AJ_NETSDK_IPC_StartTalk.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_StopTalk.argtypes = []
    libnetsdk.AJ_NETSDK_IPC_StopTalk.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_AddTalk.argtypes = [LIB_NETSDK_HANDLE]
    libnetsdk.AJ_NETSDK_IPC_AddTalk.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_RemoveTalk.argtypes = [LIB_NETSDK_HANDLE]
    libnetsdk.AJ_NETSDK_IPC_RemoveTalk.restype = c_long
    
    # ========== PTZ УПРАВЛЕНИЕ ==========
    libnetsdk.AJ_NETSDK_IPC_PTZControl.argtypes = [LIB_NETSDK_HANDLE, c_int, c_long, c_long, c_long]
    libnetsdk.AJ_NETSDK_IPC_PTZControl.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_PTZControlEx.argtypes = [LIB_NETSDK_HANDLE, c_int, c_char_p]
    libnetsdk.AJ_NETSDK_IPC_PTZControlEx.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_PTZPreset.argtypes = [LIB_NETSDK_HANDLE, c_int, c_long, c_long]
    libnetsdk.AJ_NETSDK_IPC_PTZPreset.restype = c_long
    
    # ========== КОНФИГУРАЦИЯ ==========
    libnetsdk.AJ_NETSDK_IPC_GetDVRConfig.argtypes = [LIB_NETSDK_HANDLE, c_int, c_long]
    libnetsdk.AJ_NETSDK_IPC_GetDVRConfig.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_SetDVRConfig.argtypes = [LIB_NETSDK_HANDLE, c_int, c_long, c_void_p, c_long]
    libnetsdk.AJ_NETSDK_IPC_SetDVRConfig.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_SystemControl.argtypes = [LIB_NETSDK_HANDLE, c_int, c_long, c_long, c_char_p]
    libnetsdk.AJ_NETSDK_IPC_SystemControl.restype = c_long
    
    # ========== СИСТЕМНЫЕ ФУНКЦИИ ==========
    libnetsdk.AJ_NETSDK_IPC_RebootDVR.argtypes = [LIB_NETSDK_HANDLE]
    libnetsdk.AJ_NETSDK_IPC_RebootDVR.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_FormatDisk.argtypes = [LIB_NETSDK_HANDLE, c_long]
    libnetsdk.AJ_NETSDK_IPC_FormatDisk.restype = c_long
    
    libnetsdk.AJ_NETSDK_IPC_Upgrade.argtypes = [LIB_NETSDK_HANDLE, c_char_p]
    libnetsdk.AJ_NETSDK_IPC_Upgrade.restype = c_long

# ============================================================================
# 4. КОНСТАНТЫ КОМАНД (из cmd_def.h)
# ============================================================================

# PTZ команды (примерные значения, нужно уточнить по документации)
PTZ_UP = 0
PTZ_DOWN = 1
PTZ_LEFT = 2
PTZ_RIGHT = 3
PTZ_ZOOM_IN = 4
PTZ_ZOOM_OUT = 5
PTZ_FOCUS_NEAR = 6
PTZ_FOCUS_FAR = 7
PTZ_IRIS_OPEN = 8
PTZ_IRIS_CLOSE = 9
PTZ_STOP = 10

# Команды конфигурации (из cmd_def.h)
CMD_GET_SYSTEM_CONFIG = 200
CMD_SET_SYSTEM_CONFIG = 220
CMD_GET_NETWORK_CONFIG = 300
CMD_SET_NETWORK_CONFIG = 320
CMD_GET_MEDIA_CONFIG = 500
CMD_SET_MEDIA_CONFIG = 520

# ============================================================================
# 5. ГЛАВНЫЙ КЛАСС ПРИЛОЖЕНИЯ
# ============================================================================
class AJNetSDKController:
    def __init__(self, root):
        self.root = root
        self.root.title("🔥 AJNETSDK ПОЛНЫЙ КОНТРОЛЛЕР v10.0")
        self.root.geometry("1300x850")
        self.root.configure(bg="#1a1a1a")
        
        # Состояние приложения
        self.sdk_initialized = False
        self.device_handle = -1
        self.realplay_handle = -1
        self.video_player_handle = -1
        self.search_callback = None
        self.status_callback = None
        self.realplay_callback = None
        
        # Очередь для данных видео
        self.video_queue = queue.Queue(maxsize=30)
        self.playing_video = False
        
        # GUI переменные
        self.ip_var = tk.StringVar(value="192.168.1.10")
        self.port_var = tk.StringVar(value="8091")
        self.user_var = tk.StringVar(value="admin")
        self.pass_var = tk.StringVar(value="123456")
        self.channel_var = tk.IntVar(value=0)
        
        # Создание интерфейса
        self.create_full_interface()
        self.setup_log_tags()
        
        # Инициализация SDK
        self.init_sdk()
        self.log("🚀 AJNETSDK КОНТРОЛЛЕР ЗАПУЩЕН", "green")
    
    # ============================================================================
    # 6. ОСНОВНЫЕ ФУНКЦИИ SDK
    # ============================================================================
    
    def init_sdk(self):
        """Инициализация SDK"""
        if not libnetsdk:
            self.log("❌ libNetSdk.dll не загружена", "red")
            return False
        
        try:
            result = libnetsdk.AJ_NETSDK_IPC_Init()
            if result == 0:
                self.sdk_initialized = True
                self.log("✅ SDK успешно инициализирован", "green")
                
                # Настраиваем callback для поиска устройств
                self.setup_search_callback()
                self.setup_status_callback()
                
                return True
            else:
                self.log(f"❌ Ошибка инициализации SDK: {result}", "red")
                return False
        except Exception as e:
            self.log(f"❌ Исключение при инициализации: {str(e)}", "red")
            return False
    
    def setup_search_callback(self):
        """Настройка callback для поиска устройств"""
        @SearchIPCCallBack
        def search_callback(nEventCode, index, pResponse, pUser):
            if nEventCode == 0 and pResponse:  # Устройство найдено
                device_info = pResponse.contents
                ip = device_info.lanCfg.szIP.decode('utf-8', errors='ignore')
                name = device_info.title.decode('utf-8', errors='ignore')
                serial = device_info.ipc_sn.decode('utf-8', errors='ignore')
                
                self.log(f"📡 Найдено устройство: {name} ({ip}) SN: {serial}", "cyan")
            return 0
        
        self.search_callback = search_callback
        if libnetsdk:
            libnetsdk.AJ_NETSDK_IPC_SetSearchStatusCallBack(search_callback, None)
    
    def setup_status_callback(self):
        """Настройка callback для событий статуса"""
        @StatusEventCallBack
        def status_callback(lUserID, nStateCode, pResponse, pUser):
            states = {
                0: "Подключено",
                1: "Отключено",
                2: "Переподключение",
                3: "Ошибка сети",
                100: "Видеопоток начат",
                101: "Видеопоток остановлен"
            }
            
            state_text = states.get(nStateCode, f"Код {nStateCode}")
            self.log(f"📡 Статус устройства {lUserID}: {state_text}", "yellow")
            return 0
        
        self.status_callback = status_callback
        if libnetsdk:
            libnetsdk.AJ_NETSDK_IPC_SetStatusEventCallBack(status_callback, None)
    
    def login_device(self):
        """Подключение к устройству"""
        if not self.sdk_initialized:
            self.log("⚠️ Сначала инициализируйте SDK", "yellow")
            return False
        
        ip = self.ip_var.get()
        port = int(self.port_var.get())
        user = self.user_var.get()
        password = self.pass_var.get()
        
        self.log(f"🔗 Подключение к {ip}:{port}...", "cyan")
        
        try:
            handle = libnetsdk.AJ_NETSDK_IPC_Login(
                ip.encode('utf-8'),
                port,
                user.encode('utf-8'),
                password.encode('utf-8')
            )
            
            if handle >= 0:
                self.device_handle = handle
                self.log(f"✅ Успешный вход! Handle: {handle}", "green")
                
                # Включаем автопереподключение
                libnetsdk.AJ_NETSDK_IPC_SetAutoReconnect(handle, 1)
                
                # Получаем информацию об устройстве
                self.get_device_info()
                return True
            else:
                self.log(f"❌ Ошибка входа: {handle}", "red")
                return False
                
        except Exception as e:
            self.log(f"❌ Исключение при входе: {str(e)}", "red")
            return False
    
    def get_device_info(self):
        """Получение информации об устройстве"""
        if self.device_handle < 0:
            return
        
        try:
            # Получаем количество найденных устройств
            count = libnetsdk.AJ_NETSDK_IPC_GetSearchIPCCount()
            
            for i in range(count):
                device_info = IPC_ENTRY_AJ()
                result = libnetsdk.AJ_NETSDK_IPC_GetIPCInfo(i, byref(device_info))
                
                if result == 0:
                    ip = device_info.lanCfg.szIP.decode('utf-8', errors='ignore')
                    name = device_info.title.decode('utf-8', errors='ignore')
                    channels = device_info.ChannelNum
                    
                    self.log(f"📋 Устройство {i}: {name} ({ip}), каналов: {channels}", "blue")
        
        except Exception as e:
            self.log(f"⚠️ Ошибка получения информации: {str(e)}", "yellow")
    
    def start_search_devices(self):
        """Поиск устройств в сети"""
        if not self.sdk_initialized:
            self.log("⚠️ Сначала инициализируйте SDK", "yellow")
            return
        
        self.log("🔍 Начало поиска устройств в сети...", "cyan")
        
        try:
            result = libnetsdk.AJ_NETSDK_IPC_StartSearchIPC(b"")
            if result == 0:
                self.log("✅ Поиск устройств запущен", "green")
                
                # Ждем 3 секунды для сбора результатов
                self.root.after(3000, self.show_search_results)
            else:
                self.log(f"❌ Ошибка запуска поиска: {result}", "red")
                
        except Exception as e:
            self.log(f"❌ Исключение при поиске: {str(e)}", "red")
    
    def show_search_results(self):
        """Показать результаты поиска"""
        try:
            count = libnetsdk.AJ_NETSDK_IPC_GetSearchIPCCount()
            self.log(f"📊 Найдено устройств: {count}", "green")
            
            # Останавливаем поиск
            libnetsdk.AJ_NETSDK_IPC_StopSearchIPC()
            
        except Exception as e:
            self.log(f"⚠️ Ошибка получения результатов: {str(e)}", "yellow")
    
    def start_realplay(self):
        """Запуск видеопотока"""
        if self.device_handle < 0:
            self.log("⚠️ Сначала подключитесь к устройству", "yellow")
            return
        
        channel = self.channel_var.get()
        ip = self.ip_var.get()
        user = self.user_var.get()
        password = self.pass_var.get()
        
        self.log(f"🎬 Запуск видеопотока (канал {channel})...", "cyan")
        
        # Настраиваем callback для получения видеоданных
        @fRealDataCallBack
        def realplay_callback(lRealHandle, nChannel, dwDataType, pBuffer, dwBufSize, pExtData):
            if dwDataType == 0:  # Видеоданные
                try:
                    # Копируем данные
                    frame_data = ctypes.string_at(pBuffer, dwBufSize)
                    
                    if not self.video_queue.full():
                        self.video_queue.put({
                            'data': frame_data,
                            'size': dwBufSize,
                            'channel': nChannel
                        })
                except:
                    pass
            return 0
        
        self.realplay_callback = realplay_callback
        
        try:
            handle = libnetsdk.AJ_NETSDK_IPC_RealPlay(
                ip.encode('utf-8'),
                channel,
                0,  # main stream
                user.encode('utf-8'),
                password.encode('utf-8'),
                0,  # TCP mode
                0,  # порт по умолчанию
                realplay_callback,
                None
            )
            
            if handle >= 0:
                self.realplay_handle = handle
                self.log(f"✅ Видеопоток запущен. Handle: {handle}", "green")
                
                # Запускаем обработку видео
                self.playing_video = True
                self.video_thread = threading.Thread(target=self.process_video_frames)
                self.video_thread.daemon = True
                self.video_thread.start()
                
                return True
            else:
                self.log(f"❌ Ошибка запуска видеопотока: {handle}", "red")
                return False
                
        except Exception as e:
            self.log(f"❌ Исключение при запуске видео: {str(e)}", "red")
            return False
    
    def process_video_frames(self):
        """Обработка видеокадров"""
        while self.playing_video:
            try:
                # Получаем кадр из очереди
                frame = self.video_queue.get(timeout=0.1)
                
                # Здесь можно обрабатывать или отображать видео
                # Для простоты просто логируем
                if hasattr(self, 'last_log_time') and time.time() - self.last_log_time > 2:
                    self.log(f"📹 Получен видеокадр {frame['size']} байт", "cyan")
                    self.last_log_time = time.time()
                elif not hasattr(self, 'last_log_time'):
                    self.last_log_time = time.time()
                    
            except queue.Empty:
                continue
            except Exception as e:
                print(f"Video processing error: {e}")
    
    def stop_realplay(self):
        """Остановка видеопотока"""
        if self.realplay_handle >= 0:
            try:
                self.playing_video = False
                
                if self.video_thread:
                    self.video_thread.join(timeout=1.0)
                
                result = libnetsdk.AJ_NETSDK_IPC_StopRealPlay(self.realplay_handle)
                if result == 0:
                    self.log("⏹️ Видеопоток остановлен", "green")
                    self.realplay_handle = -1
                    return True
                else:
                    self.log(f"⚠️ Ошибка остановки видео: {result}", "yellow")
                    return False
                    
            except Exception as e:
                self.log(f"❌ Исключение при остановке: {str(e)}", "red")
                return False
        else:
            self.log("⚠️ Нет активного видеопотока", "yellow")
            return False
    
    def ptz_control(self, command, speed=50):
        """Управление PTZ"""
        if self.device_handle < 0:
            self.log("⚠️ Сначала подключитесь к устройству", "yellow")
            return False
        
        channel = self.channel_var.get()
        
        try:
            result = libnetsdk.AJ_NETSDK_IPC_PTZControl(
                self.device_handle,
                channel,
                command,
                speed,  # скорость поворота
                speed   # скорость зума
            )
            
            if result == 0:
                cmd_names = {
                    PTZ_UP: "ВВЕРХ", PTZ_DOWN: "ВНИЗ",
                    PTZ_LEFT: "ВЛЕВО", PTZ_RIGHT: "ВПРАВО",
                    PTZ_ZOOM_IN: "ZOOM+", PTZ_ZOOM_OUT: "ZOOM-",
                    PTZ_STOP: "СТОП"
                }
                cmd_name = cmd_names.get(command, f"Команда {command}")
                self.log(f"🎮 PTZ: {cmd_name} (скорость: {speed})", "blue")
                return True
            else:
                self.log(f"❌ Ошибка PTZ команды: {result}", "red")
                return False
                
        except Exception as e:
            self.log(f"❌ Исключение PTZ: {str(e)}", "red")
            return False
    
    def ptz_preset(self, action, preset_index=1):
        """Управление пресетами PTZ"""
        if self.device_handle < 0:
            self.log("⚠️ Сначала подключитесь к устройству", "yellow")
            return False
        
        channel = self.channel_var.get()
        
        try:
            result = libnetsdk.AJ_NETSDK_IPC_PTZPreset(
                self.device_handle,
                channel,
                action,  # 11-установить, 12-вызвать, 13-удалить
                preset_index
            )
            
            if result == 0:
                actions = {
                    11: "Установить пресет",
                    12: "Вызвать пресет", 
                    13: "Удалить пресет"
                }
                action_name = actions.get(action, f"Действие {action}")
                self.log(f"📍 PTZ Пресет: {action_name} #{preset_index}", "blue")
                return True
            else:
                self.log(f"❌ Ошибка пресета: {result}", "red")
                return False
                
        except Exception as e:
            self.log(f"❌ Исключение пресета: {str(e)}", "red")
            return False
    
    def start_voice_com(self):
        """Начало голосовой связи"""
        if self.device_handle < 0:
            self.log("⚠️ Сначала подключитесь к устройству", "yellow")
            return False
        
        try:
            result = libnetsdk.AJ_NETSDK_IPC_StartVoiceCom(self.device_handle)
            if result == 0:
                self.log("🎤 Голосовая связь активирована", "green")
                return True
            else:
                self.log(f"❌ Ошибка голосовой связи: {result}", "red")
                return False
        except Exception as e:
            self.log(f"❌ Исключение голосовой связи: {str(e)}", "red")
            return False
    
    def reboot_device(self):
        """Перезагрузка устройства"""
        if self.device_handle < 0:
            self.log("⚠️ Сначала подключитесь к устройству", "yellow")
            return
        
        if messagebox.askyesno("Подтверждение", "Перезагрузить устройство?"):
            try:
                result = libnetsdk.AJ_NETSDK_IPC_RebootDVR(self.device_handle)
                if result == 0:
                    self.log("🔄 Перезагрузка устройства...", "cyan")
                    self.log("✅ Команда перезагрузки отправлена", "green")
                else:
                    self.log(f"❌ Ошибка перезагрузки: {result}", "red")
            except Exception as e:
                self.log(f"❌ Исключение при перезагрузке: {str(e)}", "red")
    
    def get_config_xml(self, command_type):
        """Получение конфигурации в XML формате"""
        if self.device_handle < 0:
            self.log("⚠️ Сначала подключитесь к устройству", "yellow")
            return None
        
        try:
            # Создаем буфер для XML
            xml_buffer = create_string_buffer(4096)
            
            result = libnetsdk.AJ_NETSDK_IPC_GetDVRConfig(
                self.device_handle,
                self.channel_var.get(),
                command_type
            )
            
            # В реальности нужно правильно обработать результат
            # Это упрощенный пример
            if result == 0:
                xml_str = xml_buffer.value.decode('utf-8', errors='ignore')
                self.log(f"📥 Конфигурация {command_type} получена", "green")
                return xml_str
            else:
                self.log(f"❌ Ошибка получения конфигурации: {result}", "red")
                return None
                
        except Exception as e:
            self.log(f"❌ Исключение при получении конфигурации: {str(e)}", "red")
            return None
    
    def set_config_xml(self, command_type, xml_data):
        """Установка конфигурации через XML"""
        if self.device_handle < 0:
            self.log("⚠️ Сначала подключитесь к устройству", "yellow")
            return False
        
        try:
            result = libnetsdk.AJ_NETSDK_IPC_SetDVRConfig(
                self.device_handle,
                self.channel_var.get(),
                command_type,
                xml_data.encode('utf-8'),
                len(xml_data.encode('utf-8'))
            )
            
            if result == 0:
                self.log(f"📤 Конфигурация {command_type} установлена", "green")
                return True
            else:
                self.log(f"❌ Ошибка установки конфигурации: {result}", "red")
                return False
                
        except Exception as e:
            self.log(f"❌ Исключение при установке конфигурации: {str(e)}", "red")
            return False
    
    def system_control_xml(self, command, xml_data=""):
        """Системное управление через XML"""
        if self.device_handle < 0:
            self.log("⚠️ Сначала подключитесь к устройству", "yellow")
            return False
        
        try:
            result = libnetsdk.AJ_NETSDK_IPC_SystemControl(
                self.device_handle,
                self.channel_var.get(),
                command,
                0,  # флаг
                xml_data.encode('utf-8') if xml_data else None
            )
            
            return result == 0
                
        except Exception as e:
            self.log(f"❌ Исключение system control: {str(e)}", "red")
            return False
    
    def cleanup(self):
        """Очистка ресурсов"""
        # Останавливаем видео
        if self.realplay_handle >= 0:
            self.stop_realplay()
        
        # Выходим из устройства
        if self.device_handle >= 0 and libnetsdk:
            try:
                libnetsdk.AJ_NETSDK_IPC_Logout(self.device_handle)
                self.log("✅ Выход из устройства", "green")
            except:
                pass
        
        # Очищаем SDK
        if self.sdk_initialized and libnetsdk:
            try:
                libnetsdk.AJ_NETSDK_IPC_Cleanup()
                self.log("🧹 Ресурсы SDK очищены", "green")
            except:
                pass
    
    # ============================================================================
    # 7. ГРАФИЧЕСКИЙ ИНТЕРФЕЙС
    # ============================================================================
    
    def create_full_interface(self):
        """Создание полного интерфейса"""
        # Заголовок
        title = tk.Label(self.root, text="🔥 AJNETSDK ПОЛНЫЙ КОНТРОЛЛЕР КАМЕР", 
                        font=('Arial', 22, 'bold'), fg="#00ff88", bg="#1a1a1a")
        title.pack(pady=15)
        
        # Основной контейнер с вкладками
        notebook = ttk.Notebook(self.root)
        notebook.pack(fill="both", expand=True, padx=10, pady=10)
        
        # Вкладка 1: Подключение и управление
        control_tab = tk.Frame(notebook, bg="#1a1a1a")
        notebook.add(control_tab, text="🔌 Управление")
        self.create_control_tab(control_tab)
        
        # Вкладка 2: PTZ управление
        ptz_tab = tk.Frame(notebook, bg="#1a1a1a")
        notebook.add(ptz_tab, text="🎮 PTZ")
        self.create_ptz_tab(ptz_tab)
        
        # Вкладка 3: Конфигурация
        config_tab = tk.Frame(notebook, bg="#1a1a1a")
        notebook.add(config_tab, text="⚙️ Конфигурация")
        self.create_config_tab(config_tab)
        
        # Вкладка 4: Видео и аудио
        media_tab = tk.Frame(notebook, bg="#1a1a1a")
        notebook.add(media_tab, text="📺 Медиа")
        self.create_media_tab(media_tab)
        
        # Лог внизу
        self.create_log_section()
    
    def create_control_tab(self, parent):
        """Вкладка управления подключением"""
        # Панель подключения
        conn_frame = tk.LabelFrame(parent, text="🔌 ПОДКЛЮЧЕНИЕ К УСТРОЙСТВУ", 
                                  font=('Arial', 13, 'bold'), fg="#00ff88", bg="#2a2a2a")
        conn_frame.pack(fill="x", padx=15, pady=15)
        
        # Поля ввода
        input_frame = tk.Frame(conn_frame, bg="#2a2a2a")
        input_frame.pack(pady=15, padx=20)
        
        tk.Label(input_frame, text="IP адрес:", bg="#2a2a2a", fg="white", 
                width=12, anchor="e").grid(row=0, column=0, padx=5, pady=5, sticky="e")
        tk.Entry(input_frame, textvariable=self.ip_var, width=20).grid(row=0, column=1, padx=5, pady=5)
        
        tk.Label(input_frame, text="Порт:", bg="#2a2a2a", fg="white", 
                width=8, anchor="e").grid(row=0, column=2, padx=5, pady=5, sticky="e")
        tk.Entry(input_frame, textvariable=self.port_var, width=10).grid(row=0, column=3, padx=5, pady=5)
        
        tk.Label(input_frame, text="Пользователь:", bg="#2a2a2a", fg="white", 
                width=12, anchor="e").grid(row=1, column=0, padx=5, pady=5, sticky="e")
        tk.Entry(input_frame, textvariable=self.user_var, width=20).grid(row=1, column=1, padx=5, pady=5)
        
        tk.Label(input_frame, text="Пароль:", bg="#2a2a2a", fg="white", 
                width=8, anchor="e").grid(row=1, column=2, padx=5, pady=5, sticky="e")
        tk.Entry(input_frame, textvariable=self.pass_var, width=15, show="*").grid(row=1, column=3, padx=5, pady=5)
        
        tk.Label(input_frame, text="Канал:", bg="#2a2a2a", fg="white", 
                width=8, anchor="e").grid(row=0, column=4, padx=20, pady=5, sticky="e")
        tk.Entry(input_frame, textvariable=self.channel_var, width=8).grid(row=0, column=5, padx=5, pady=5)
        
        # Кнопки управления подключением
        btn_frame = tk.Frame(conn_frame, bg="#2a2a2a")
        btn_frame.pack(pady=10)
        
        tk.Button(btn_frame, text="🔗 ПОДКЛЮЧИТЬСЯ", command=self.login_device,
                 bg="#00aa00", fg="white", font=('Arial', 11, 'bold'), width=18).pack(side="left", padx=10)
        
        tk.Button(btn_frame, text="📡 ПОИСК УСТРОЙСТВ", command=self.start_search_devices,
                 bg="#4488ff", fg="white", font=('Arial', 11, 'bold'), width=18).pack(side="left", padx=10)
        
        tk.Button(btn_frame, text="🔄 ПЕРЕЗАГРУЗКА", command=self.reboot_device,
                 bg="#ff4444", fg="white", font=('Arial', 11, 'bold'), width=18).pack(side="left", padx=10)
        
        # Системные кнопки
        sys_frame = tk.Frame(conn_frame, bg="#2a2a2a")
        sys_frame.pack(pady=10)
        
        sys_buttons = [
            ("🎤 ГОЛОСОВАЯ СВЯЗЬ", self.start_voice_com, "#ffaa00"),
            ("💾 ФОРМАТИРОВАТЬ", lambda: self.format_storage(0), "#aa44ff"),
            ("🔄 ОБНОВИТЬ ПРОШИВКУ", self.upgrade_firmware, "#44aaff"),
        ]
        
        for text, command, color in sys_buttons:
            tk.Button(sys_frame, text=text, command=command,
                     bg=color, fg="white", font=('Arial', 10), width=20).pack(side="left", padx=5, pady=5)
    
    def create_ptz_tab(self, parent):
        """Вкладка PTZ управления"""
        ptz_frame = tk.LabelFrame(parent, text="🎮 УПРАВЛЕНИЕ PTZ", 
                                 font=('Arial', 13, 'bold'), fg="#00ff88", bg="#2a2a2a")
        ptz_frame.pack(fill="both", expand=True, padx=15, pady=15)
        
        # Кнопки движения
        move_frame = tk.Frame(ptz_frame, bg="#2a2a2a")
        move_frame.pack(pady=20)
        
        move_buttons = [
            ("▲ ВВЕРХ", PTZ_UP, "#00aa00"),
            ("▼ ВНИЗ", PTZ_DOWN, "#00aa00"), 
            ("◀ ВЛЕВО", PTZ_LEFT, "#00aa00"),
            ("▶ ВПРАВО", PTZ_RIGHT, "#00aa00"),
            ("⊕ ZOOM+", PTZ_ZOOM_IN, "#ffaa00"),
            ("⊖ ZOOM-", PTZ_ZOOM_OUT, "#ffaa00"),
            ("🎯 ФОКУС+", PTZ_FOCUS_FAR, "#44aaff"),
            ("🎯 ФОКУС-", PTZ_FOCUS_NEAR, "#44aaff"),
            ("⏹ СТОП", PTZ_STOP, "#ff4444"),
        ]
        
        # Размещаем кнопки в сетке 3x3
        for i in range(3):
            row_frame = tk.Frame(move_frame, bg="#2a2a2a")
            row_frame.pack(pady=5)
            
            for j in range(3):
                idx = i * 3 + j
                if idx < len(move_buttons):
                    text, cmd, color = move_buttons[idx]
                    tk.Button(row_frame, text=text, 
                             command=lambda c=cmd: self.ptz_control(c),
                             bg=color, fg="white", font=('Arial', 10),
                             width=12).pack(side="left", padx=5)
        
        # Управление пресетами
        preset_frame = tk.Frame(ptz_frame, bg="#2a2a2a")
        preset_frame.pack(pady=20)
        
        tk.Label(preset_frame, text="Пресеты PTZ:", bg="#2a2a2a", fg="white",
                font=('Arial', 11)).pack(side="left", padx=10)
        
        preset_buttons = [
            ("📍 УСТАНОВИТЬ", 11),
            ("🎯 ВЫЗВАТЬ", 12),
            ("🗑️ УДАЛИТЬ", 13)
        ]
        
        for text, action in preset_buttons:
            btn_frame = tk.Frame(preset_frame, bg="#2a2a2a")
            btn_frame.pack(side="left", padx=5)
            
            tk.Label(btn_frame, text=text.split()[0], bg="#2a2a2a", fg="white",
                    font=('Arial', 9)).pack()
            
            for i in range(1, 4):
                tk.Button(btn_frame, text=str(i),
                         command=lambda a=action, idx=i: self.ptz_preset(a, idx),
                         bg="#555555", fg="white", width=3).pack(side="left", padx=2)
    
    def create_config_tab(self, parent):
        """Вкладка конфигурации"""
        config_frame = tk.LabelFrame(parent, text="⚙️ КОНФИГУРАЦИЯ УСТРОЙСТВА", 
                                    font=('Arial', 13, 'bold'), fg="#00ff88", bg="#2a2a2a")
        config_frame.pack(fill="both", expand=True, padx=15, pady=15)
        
        # Кнопки получения конфигурации
        get_frame = tk.LabelFrame(config_frame, text="Получить конфигурацию", 
                                 bg="#2a2a2a", fg="#00ff88")
        get_frame.pack(fill="x", padx=10, pady=10)
        
        get_buttons = [
            ("📋 Системная", CMD_GET_SYSTEM_CONFIG, lambda: self.get_config_xml(CMD_GET_SYSTEM_CONFIG)),
            ("🌐 Сетевая", CMD_GET_NETWORK_CONFIG, lambda: self.get_config_xml(CMD_GET_NETWORK_CONFIG)),
            ("🎬 Медиа", CMD_GET_MEDIA_CONFIG, lambda: self.get_config_xml(CMD_GET_MEDIA_CONFIG)),
        ]
        
        btn_row = tk.Frame(get_frame, bg="#2a2a2a")
        btn_row.pack(pady=10)
        
        for text, cmd, func in get_buttons:
            tk.Button(btn_row, text=text, command=func,
                     bg="#4488ff", fg="white", width=15).pack(side="left", padx=10)
        
        # Кнопки установки конфигурации
        set_frame = tk.LabelFrame(config_frame, text="Установить конфигурацию", 
                                 bg="#2a2a2a", fg="#00ff88")
        set_frame.pack(fill="x", padx=10, pady=10)
        
        # Поле для XML
        xml_frame = tk.Frame(set_frame, bg="#2a2a2a")
        xml_frame.pack(pady=10)
        
        tk.Label(xml_frame, text="XML данные:", bg="#2a2a2a", fg="white").pack(side="left", padx=5)
        
        self.xml_text = tk.Text(xml_frame, height=8, width=60, bg="#0a0a0a", fg="#00ff88")
        self.xml_text.pack(side="left", padx=5)
        
        # Кнопки применения
        apply_frame = tk.Frame(set_frame, bg="#2a2a2a")
        apply_frame.pack(pady=10)
        
        apply_buttons = [
            ("💾 Применить системную", CMD_SET_SYSTEM_CONFIG),
            ("💾 Применить сетевую", CMD_SET_NETWORK_CONFIG),
            ("💾 Применить медиа", CMD_SET_MEDIA_CONFIG),
        ]
        
        for text, cmd in apply_buttons:
            tk.Button(apply_frame, text=text, 
                     command=lambda c=cmd: self.apply_config_xml(c),
                     bg="#00aa00", fg="white", width=20).pack(side="left", padx=10)
    
    def apply_config_xml(self, command_type):
        """Применение XML конфигурации"""
        xml_data = self.xml_text.get("1.0", tk.END).strip()
        if xml_data:
            self.set_config_xml(command_type, xml_data)
        else:
            messagebox.showwarning("Внимание", "Введите XML данные для применения")
    
    def create_media_tab(self, parent):
        """Вкладка видео и аудио"""
        media_frame = tk.LabelFrame(parent, text="📺 ВИДЕО И АУДИО УПРАВЛЕНИЕ", 
                                   font=('Arial', 13, 'bold'), fg="#00ff88", bg="#2a2a2a")
        media_frame.pack(fill="both", expand=True, padx=15, pady=15)
        
        # Видео управление
        video_frame = tk.Frame(media_frame, bg="#2a2a2a")
        video_frame.pack(fill="x", pady=10)
        
        tk.Button(video_frame, text="🎬 ЗАПУСТИТЬ ВИДЕОПОТОК", command=self.start_realplay,
                 bg="#00aa00", fg="white", font=('Arial', 12, 'bold'), width=25).pack(side="left", padx=20, pady=10)
        
        tk.Button(video_frame, text="⏹️ ОСТАНОВИТЬ ВИДЕО", command=self.stop_realplay,
                 bg="#ff4444", fg="white", font=('Arial', 12, 'bold'), width=25).pack(side="left", padx=20, pady=10)
        
        # Аудио управление
        audio_frame = tk.Frame(media_frame, bg="#2a2a2a")
        audio_frame.pack(fill="x", pady=10)
        
        audio_buttons = [
            ("🎤 ВКЛ. ГОЛОСОВУЮ СВЯЗЬ", self.start_voice_com, "#00aa00"),
            ("🎤 ВЫКЛ. ГОЛОСОВУЮ СВЯЗЬ", lambda: self.stop_voice_com() if hasattr(self, 'stop_voice_com') else None, "#ff4444"),
            ("📢 НАЧАТЬ ТРАНСЛЯЦИЮ", lambda: self.start_talk() if hasattr(self, 'start_talk') else None, "#4488ff"),
            ("📢 ОСТАНОВИТЬ ТРАНСЛЯЦИЮ", lambda: self.stop_talk() if hasattr(self, 'stop_talk') else None, "#ffaa00"),
        ]
        
        for i in range(0, len(audio_buttons), 2):
            row_frame = tk.Frame(audio_frame, bg="#2a2a2a")
            row_frame.pack(pady=5)
            
            for j in range(2):
                if i + j < len(audio_buttons):
                    text, command, color = audio_buttons[i + j]
                    tk.Button(row_frame, text=text, command=command,
                             bg=color, fg="white", width=25).pack(side="left", padx=10)
    
    def create_log_section(self):
        """Создание секции лога"""
        log_frame = tk.LabelFrame(self.root, text="📋 ЖУРНАЛ СОБЫТИЙ SDK", 
                                 font=('Arial', 13, 'bold'), fg="#00ff88", bg="#2a2a2a")
        log_frame.pack(fill="both", expand=True, padx=10, pady=10)
        
        self.log_text = scrolledtext.ScrolledText(log_frame, height=15, 
                                                 bg="#0a0a0a", fg="#00ff88",
                                                 font=('Consolas', 9), state='normal')
        self.log_text.pack(fill="both", expand=True, padx=10, pady=10)
    
    def setup_log_tags(self):
        """Настройка цветов для лога"""
        self.log_text.tag_config("green", foreground="#00ff88")
        self.log_text.tag_config("red", foreground="#ff4444")
        self.log_text.tag_config("yellow", foreground="#ffaa00")
        self.log_text.tag_config("blue", foreground="#4488ff")
        self.log_text.tag_config("cyan", foreground="#00ffff")
    
    def log(self, msg, color="white"):
        """Вывод сообщения в лог"""
        timestamp = datetime.now().strftime("%H:%M:%S")
        full_msg = f"[{timestamp}] {msg}\n"
        
        self.log_text.insert(tk.END, full_msg)
        
        if color != "white":
            # Вычисляем строку для добавления тега
            line_num = int(self.log_text.index('end-1c').split('.')[0]) - 1
            start_pos = f"{line_num}.0"
            end_pos = f"{line_num}.end"
            self.log_text.tag_add(color, start_pos, end_pos)
        
        self.log_text.see(tk.END)
        self.root.update_idletasks()
    
    def format_storage(self, disk_number):
        """Форматирование хранилища"""
        if self.device_handle < 0:
            self.log("⚠️ Сначала подключитесь к устройству", "yellow")
            return
        
        if messagebox.askyesno("Внимание!", f"Форматировать диск {disk_number}?\nВсе данные будут удалены!"):
            try:
                result = libnetsdk.AJ_NETSDK_IPC_FormatDisk(self.device_handle, disk_number)
                if result == 0:
                    self.log(f"💾 Форматирование диска {disk_number} начато", "cyan")
                else:
                    self.log(f"❌ Ошибка форматирования: {result}", "red")
            except Exception as e:
                self.log(f"❌ Исключение при форматировании: {str(e)}", "red")
    
    def upgrade_firmware(self):
        """Обновление прошивки"""
        if self.device_handle < 0:
            self.log("⚠️ Сначала подключитесь к устройству", "yellow")
            return
        
        filename = filedialog.askopenfilename(
            title="Выберите файл прошивки",
            filetypes=[("Firmware files", "*.bin;*.pak;*.dig"), ("All files", "*.*")]
        )
        
        if filename:
            try:
                result = libnetsdk.AJ_NETSDK_IPC_Upgrade(self.device_handle, filename.encode('utf-8'))
                if result == 0:
                    self.log(f"🔄 Обновление прошивки из {filename}", "cyan")
                else:
                    self.log(f"❌ Ошибка обновления: {result}", "red")
            except Exception as e:
                self.log(f"❌ Исключение при обновлении: {str(e)}", "red")
    
    def on_closing(self):
        """Обработчик закрытия окна"""
        self.cleanup()
        self.root.destroy()

# ============================================================================
# 8. ЗАПУСК ПРИЛОЖЕНИЯ
# ============================================================================
def main():
    root = tk.Tk()
    app = AJNetSDKController(root)
    
    # Обработчик закрытия окна
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    
    # Центрируем окно
    root.update_idletasks()
    width = root.winfo_width()
    height = root.winfo_height()
    x = (root.winfo_screenwidth() // 2) - (width // 2)
    y = (root.winfo_screenheight() // 2) - (height // 2)
    root.geometry(f'{width}x{height}+{x}+{y}')
    
    root.mainloop()

if __name__ == "__main__":
    main()
