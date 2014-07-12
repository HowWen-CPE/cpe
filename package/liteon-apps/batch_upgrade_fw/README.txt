Script file name: cpe_fw_upgrade_v1.0.bat

Usage:
    1) Modify setting in cpe_fw_upgrade_v1.0.txt
       a) Firmware name
       b) Download protocol: ftp(recommended) or tftp
       c) Server IP (ftp or tftp)
       d) ftp account (if by ftp)
       e) SNMP account.
    2) Modify the CPE IP list in cpe_ip_list.txt
       The format is each IP (IPv4) occupy one line.
       Please get the CPE IP list by tool CPE discovery tool.
    3) Start ftp or tftp service
    4) Windows->Start cmd.exe--> Goto this script directory--> type cpe_fw_upgrade_v1.0.bat
       There will be log file (cpe_firmware_upgrade.log) after upgrading.

Purpose:
    Take use of this script to batch upgrade CPE firmware.


Batch upgrade scheme
    Use SNMP to trigger the CPE download firmware from specified FTP or TFTP, then write it to flash.
    The script trigger the upgrading task one by one (list in cpe_ip_list.txt).
    For getting CPE IP list, please take use of the CPE discovery tool.


Limitation
    If CPE IP address is obtained by DHCP, the upgrading result status may not correct if the device got a different IP address after upgrading firmware.
