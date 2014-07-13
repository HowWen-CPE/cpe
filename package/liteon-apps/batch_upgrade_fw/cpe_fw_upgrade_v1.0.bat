@echo off
rem -------------------------------------------------------------------------------
rem NOTICE:
rem   1) Before run script, please specify 
rem      a) the setting in cpe_fw_upgrade_v1.0.txt
rem      b) the ip list in cpe_ip_list.txt
rem   2) Start ftp (recommended) or tftp server
rem   3) Run this script in cmd.exe, instead of double click the script 
rem -------------------------------------------------------------------------------

set debug=0

set DOWNLOAD_TEST_LIST_FILE=tmp\DOWNLOAD_TEST_LIST_FILE.log
set DOWNLOAD_DONE_LIST_FILE=tmp\DOWNLOAD_DONE_LIST_FILE.log
set DOWNLOAD_FAIL_LIST_FILE=tmp\DOWNLOAD_FAIL_LIST_FILE.log
set INVALID_FW_LIST_FILE=tmp\INVALID_FW_LIST_FILE.log

echo.>> %UPGRADE_LOG_FILE%
echo.>> %UPGRADE_LOG_FILE%
echo %date:~0,10% %time:~0,8%
echo %date:~0,10% %time:~0,8% >> %UPGRADE_LOG_FILE%
echo *******************************************************
echo *       CPE Firmware Batch Upgrade Script (V1.0)      *
echo *******************************************************

echo *******************************************************>> %UPGRADE_LOG_FILE%
echo *       CPE Firmware Batch Upgrade Script (V1.0)      *>> %UPGRADE_LOG_FILE%
echo *******************************************************>> %UPGRADE_LOG_FILE%


echo Parsing configuration:
echo Parsing configuration: >> %UPGRADE_LOG_FILE%


for /f "eol=# tokens=*" %%a in (%~n0.txt) do (
    rem echo.
    if %debug%==1 echo debug 1:a=%%a. b=%%b.

    for /f "delims=# tokens=1*" %%m in ("%%a") do (
        if %debug%==1 echo debug 2:m=%%m. n=%%n.
        for /f "delims== tokens=1,2*" %%x in ("%%m") do (
            if %debug%==1 echo debug 3:x=%%x. y=%%y.
            if not "%%y"=="" (
                for /f %%k in ("%%x") do (
                    for /f "tokens=1*" %%v in ("%%y") do (
                        if "%%w"=="" (
                            ::echo .%%k.=.%%v.
                            set %%k=%%v
                        ) else (
                            ::echo .%%k.=.%%v %%w.
                            set %%k=%%v %%w
                        )
                    )
                )
            )
        )
    )
)
rem parse configuration done.


rem show configuration
if %DOWNLOAD_PROTOCOL%==tftp (
	echo     Download by tftp
	echo     TFTP_IP: %TFTP_IP%

	echo     Download by tftp  >> %UPGRADE_LOG_FILE%
	echo     TFTP_IP: %TFTP_IP%  >> %UPGRADE_LOG_FILE%

) else (
	echo     Download by ftp
	echo     Firmware name: %CPE_FIRMWARE_FILE%
	echo     FTP server IP: %FTP_IP%
	echo     FTP user name: %FTP_USERNAME%
	echo     FTP password: %FTP_PASSWORD%

	echo     Download by ftp >> %UPGRADE_LOG_FILE%
	echo     Firmware name: %CPE_FIRMWARE_FILE% >> %UPGRADE_LOG_FILE%
	echo     FTP server IP: %FTP_IP% >> %UPGRADE_LOG_FILE%
	echo     FTP user name: %FTP_USERNAME% >> %UPGRADE_LOG_FILE%
	echo     FTP password: %FTP_PASSWORD% >> %UPGRADE_LOG_FILE%

)

if "%SNMP_READ%"=="" (
	set %SNMP_READ%=public
)

if "%SNMP_WRITE%"=="" (
	set %SNMP_WRITE%=private
)

echo     SNMP_READ:  %SNMP_READ%
echo     SNMP_WIRTE: %SNMP_WRITE%

echo     SNMP_READ:  %SNMP_READ%>> %UPGRADE_LOG_FILE%

echo     SNMP_WIRTE: %SNMP_WRITE%>> %UPGRADE_LOG_FILE%

echo.

echo. >> %UPGRADE_LOG_FILE%

rem show configuration down 



rem -----------------------------------------------------
rem  Start upgrade firmware
rem -----------------------------------------------------
echo Start to upgrade firmware:


echo Start to upgrade firmware: >> %UPGRADE_LOG_FILE%

setlocal EnableDelayedExpansion
set /a total_num=0
set /a cur_index=0
set /a fail_num=0
set /a resp_err_num=0
set /a snmp_set_err_num=0
set /a snmp_set_ok_num=0
set /a no_response_num=0
set /a download_fail_num=0
set /a invalid_fw_num=0
set /a download_done_num=0
set snmp_set_ok=no


rem prepare
if not exist tmp ( mkdir tmp )
if exist %DOWNLOAD_TEST_LIST_FILE% ( del %DOWNLOAD_TEST_LIST_FILE% > tmp\null.log 2>&1 )
if exist %DOWNLOAD_DONE_LIST_FILE% ( del %DOWNLOAD_DONE_LIST_FILE% > tmp\null.log 2>&1 )
if exist %DOWNLOAD_FAIL_LIST_FILE% ( del %DOWNLOAD_FAIL_LIST_FILE% > tmp\null.log 2>&1 )
if exist %INVALID_FW_LIST_FILE% ( del %INVALID_FW_LIST_FILE%  > tmp\null.log 2>&1 )

rem upgrade firmware
for /f "eol=# tokens=*" %%i in (cpe_ip_list.txt) do (
	echo.
	echo.>> %UPGRADE_LOG_FILE%
	set /a cur_index=1+!cur_index!
	set snmp_set_ok=no
    echo     [!cur_index!] [IP]: %%i [Time]: !date:~0,10! !time:~0,8! 
    echo     [!cur_index!] [IP]: %%i [Time]: !date:~0,10! !time:~0,8! >> %UPGRADE_LOG_FILE%
    
	if  %DOWNLOAD_PROTOCOL%==tftp (
		echo     Try to connect ...   
		ping %%i -n 1 > tmp\null.log

		snmpset -v 2c -c %SNMP_WRITE% %%i  .1.3.6.1.4.1.99000.1.600.1.2.1.0 s %CPE_FIRMWARE_FILE% .1.3.6.1.4.1.99000.1.600.1.2.3.0 i 0  .1.3.6.1.4.1.99000.1.600.1.2.2.0 a %TFTP_IP% .1.3.6.1.4.1.99000.1.600.1.2.4.0 i 1 > tmp\snmp.log 2>&1
		rem snmpset -v 2c -c %SNMP_WRITE% %%i  .1.3.6.1.4.1.99000.1.600.1.2.1.0 s %CPE_FIRMWARE_FILE% .1.3.6.1.4.1.99000.1.600.1.2.3.0 i 0  .1.3.6.1.4.1.99000.1.600.1.2.2.0 a %TFTP_IP%  > tmp\snmp.log 2>&1
		
	) else (
		rem download by ftp

		snmpset -v 2c -c %SNMP_WRITE% %%i  .1.3.6.1.4.1.99000.1.600.1.2.1.0 s %CPE_FIRMWARE_FILE% .1.3.6.1.4.1.99000.1.600.1.2.3.0 i 1  .1.3.6.1.4.1.99000.1.600.1.2.2.0 a %FTP_IP% .1.3.6.1.4.1.99000.1.600.1.2.6.0 s %FTP_USERNAME% .1.3.6.1.4.1.99000.1.600.1.2.7.0 s %FTP_PASSWORD% .1.3.6.1.4.1.99000.1.600.1.2.4.0 i 1  > tmp\snmp.log 2>&1
		rem snmpset -v 2c -c %SNMP_WRITE% %%i  .1.3.6.1.4.1.99000.1.600.1.2.1.0 s %CPE_FIRMWARE_FILE% .1.3.6.1.4.1.99000.1.600.1.2.3.0 i 1  .1.3.6.1.4.1.99000.1.600.1.2.2.0 a %FTP_IP% .1.3.6.1.4.1.99000.1.600.1.2.6.0 s %FTP_USERNAME% .1.3.6.1.4.1.99000.1.600.1.2.7.0 s %FTP_PASSWORD%   > tmp\snmp.log 2>&1
	)

	set is_checked=no
	
	rem -----------------------------------------------------
	rem  check snmp status at once after set snmp
	rem -----------------------------------------------------
	for /f "delims=: tokens=1*" %%x in (tmp\snmp.log) do (
		if !is_checked!==no (
			if %debug%==1 (  echo debug 1: %%x y=%%y +++++++ )

			if "%%x"=="Timeout" (
				echo     Error: %%i no response. Please check the network and SNMP setting.
				echo     Error: %%i no response. Please check the network and SNMP setting. >> %UPGRADE_LOG_FILE%

				set snmp_set_ok=no
				set /a no_response_num=!no_response_num!+1
				set /a fail_num=!fail_num!+1
				
				if not "!no_resp_ip_list!"=="" (
					set no_resp_ip_list=!no_resp_ip_list!, %%i
				) else (
					set no_resp_ip_list=%%i
				)

				if not "!fail_ip_list!"=="" (
					set fail_ip_list=!fail_ip_list!, %%i
				) else (
					set fail_ip_list=%%i
				)

			)else if "%%x"=="Error in packet." (
				echo     Error: set %%i upgrade mib error! Upgrading is already in process?!
				echo     Error: set %%i upgrade mib error! Upgrading is already in process?! >> %UPGRADE_LOG_FILE%

				set snmp_set_ok=no
				set /a fail_num=!fail_num!+1

				set /a resp_err_num=!resp_err_num!+1
				
				if not "!resp_err_list!"=="" (
					set resp_err_list=!resp_err_list!, %%i
				) else (
					set resp_err_list=%%i
				)

				if not "!fail_ip_list!"=="" (
					set fail_ip_list=!fail_ip_list!, %%i
				) else (
					set fail_ip_list=%%i
				)
	
			)else if "%%x"=="SNMPv2-SMI" (
				if %debug%==1 ( echo debug 1: x=%%x y=%%y ......... )

				for /f "delims== tokens=1*" %%a in (tmp\snmp.log) do (
					if %debug%==1 ( echo debug 1: a=%%a b=%%b ......... )
					if "%%a"=="SNMPv2-SMI::enterprises.99000.1.600.1.2.4.0 " (
						rem SNMPv2-SMI::enterprises.99000.1.600.1.2.4.0 
						rem snmp set succeed, wait for it download done.
						rem ping -n 1 127.0.0.1 > tmp\null.log

						set /a MAX_CHECK_TIMES=20

						if %debug%==1 ( echo debug 1: call :WAIT_DOWNLOAD_DONE %%i !MAX_CHECK_TIMES! )
						call :WAIT_DOWNLOAD_DONE %%i !MAX_CHECK_TIMES!

						set snmp_set_ok=yes
						set /a snmp_set_ok_num=!snmp_set_ok_num!+1
					
					)else (
						if %debug%==1 ( echo debug 1: a=%%a b=%%b -------- )
					)
				)

				if not !snmp_set_ok!==yes (
					set /a fail_num=!fail_num!+1

					set /a snmp_set_err_num=!snmp_set_err_num!+1
					
					if not "!snmp_set_err_list!"=="" (
						set snmp_set_err_list=!snmp_set_err_list!, %%i
					) else (
						set snmp_set_err_list=%%i
					)

					if not "!fail_ip_list!"=="" (
						set fail_ip_list=!fail_ip_list!, %%i
					) else (
						set fail_ip_list=%%i
					)
					
					echo     Error: set %%i upgrade mib error! Upgrading is already in process?!
					echo     Error: set %%i upgrade mib error! Upgrading is already in process?! >> %UPGRADE_LOG_FILE%	
				)
				
			)else (
				if %debug%==1 ( echo debug 1: x=%%x y=%%y -------- )
			)

			rem should check only one time
			set is_checked=yes
		)
	)
)

set /a upgrade_ok_num=0
set /a upgrade_unknown_status_num=0
set is_checked=no

rem -----------------------------------------------------
rem  if no download ok, goto result at once
rem -----------------------------------------------------
if !download_done_num!==0 (
	if "!still_download!"=="" (
		goto :DISPLAY_RESULT
	)
)

rem -----------------------------------------------------
rem  Check the upgrade status after two minutes later
rem -----------------------------------------------------
echo.
echo. >> %UPGRADE_LOG_FILE%
echo Wait for check upgrade result
echo Wait for check upgrade result >> %UPGRADE_LOG_FILE%
ping -n 150 127.0.0.1  > tmp\null.log


rem go through the download_down list and check upgrade status
for /f "delims=, tokens=1*" %%a in  (%DOWNLOAD_DONE_LIST_FILE%)  do (
	if %debug%==1 ( echo debug 1: a=%%a )
	
	set is_checked=no
	
	if not "%%a"=="" (
		if %debug%==1 ( echo debug 1: snmpget -v 2c -c %SNMP_READ% %%a .1.3.6.1.4.1.99000.1.600.1.2.5.0 )
		snmpget -v 2c -c %SNMP_READ% %%a .1.3.6.1.4.1.99000.1.600.1.2.5.0 > tmp\tmp.log 2>&1

		if %debug%==1 ( echo debug 1: check %%a in tmp\tmp.log )

		for /f "delims=: tokens=2*" %%x in (tmp\tmp.log) do (
			if %debug%==1 ( echo debug 1: echo x=%%x y=%%y z=%%z )
			if !is_checked!==no (
				if "%%y"==" 1" (
					if %debug%==1 ( echo debug 1: echo "OK" )
					
					set /a upgrade_ok_num=!upgrade_ok_num!+1

					if not "!upgrade_ok_ip_list!"=="" (
						set upgrade_ok_ip_list=!upgrade_ok_ip_list!, %%a
					) else (
						set upgrade_ok_ip_list=%%a
					)
				) else (
					if %debug%==1 (echo debug 1:  %%x .... unknown status )
					
					set /a upgrade_unknown_status_num=!upgrade_unknown_status_num!+1

					if not "!upgrade_fail_ip_list!"=="" (
						set upgrade_fail_ip_list=!upgrade_fail_ip_list!, %%a
					) else (
						set upgrade_fail_ip_list=%%a
					)
				)

				rem only check one line
				set is_checked=yes
			)
		)
	)
)

rem -----------------------------------------------------
rem  Display result
rem -----------------------------------------------------
:DISPLAY_RESULT
	rem fail num also include invalid fw num and download fail num
	set /a fail_num=!fail_num!+!invalid_fw_num!+!download_fail_num!
	
	rem display summary
	echo.
	echo. >> %UPGRADE_LOG_FILE%
	echo --------------------------------------------------- 
	echo --------------------------------------------------- >> %UPGRADE_LOG_FILE%
	echo Complete at %date:~0,10% %time:~0,8%
	echo Complete at %date:~0,10% %time:~0,8% >> %UPGRADE_LOG_FILE%
	echo     Total device number: !cur_index!
	echo     Total device number: !cur_index! >> %UPGRADE_LOG_FILE%
	echo     Upgrade OK units [!upgrade_ok_num!]: !upgrade_ok_ip_list!
	echo     Upgrade OK units [!upgrade_ok_num!]: !upgrade_ok_ip_list! >> %UPGRADE_LOG_FILE%
	if %debug%==1 ( echo debug 1: fail_num=!fail_num! invalid_fw_num=!invalid_fw_num! download_fail_num=!download_fail_num! )
	if %debug%==1 ( echo debug 1: fail_ip_list=!fail_ip_list!,invalid_fw_list=!invalid_fw_list!,download_fail_list=!download_fail_list! )
	if %debug%==1 ( echo debug 1: upgrade_fail_ip_list=!upgrade_fail_ip_list! )
	if %debug%==1 ( echo debug 1: SNMP set OK number: !snmp_set_ok_num! )
	if %debug%==1 ( echo debug 1: Download done number: !download_done_num! )

	if not "!fail_num!"=="0" (
		if not "!invalid_fw_num!"=="0" (
			set fail_ip_list=!fail_ip_list!, !invalid_fw_list!
		)
		if not "!download_fail_num!"=="0" (

			set fail_ip_list=!fail_ip_list!, !download_fail_list!
		)

		echo     Upgrade fail units[!fail_num!]: !fail_ip_list!
		echo     Upgrade fail units[!fail_num!]: !fail_ip_list! >> %UPGRADE_LOG_FILE%

		echo     Fail reason:
		echo     Fail reason: >> %UPGRADE_LOG_FILE%
	)

	if not !upgrade_unknown_status_num!==0 (
		echo         Unknown status units [!upgrade_unknown_status_num!]: !upgrade_fail_ip_list!
		echo         Unknown status units [!upgrade_unknown_status_num!]: !upgrade_fail_ip_list! >> %UPGRADE_LOG_FILE%
	)

	if not !no_response_num!==0 (
		echo         No response units[!no_response_num!]: !no_resp_ip_list!
		echo         No response units[!no_response_num!]: !no_resp_ip_list! >> %UPGRADE_LOG_FILE%
	)

	if not !download_fail_num!==0 (
		echo         Download fail units[!download_fail_num!]: !download_fail_list!
		echo         Download fail units[!download_fail_num!]: !download_fail_list!>> %UPGRADE_LOG_FILE%
	)

	if not !invalid_fw_num!==0 (
		echo         Invalid firmware units[!invalid_fw_num!]: !invalid_fw_list!
		echo         Invalid firmware units[!invalid_fw_num!]: !invalid_fw_list! >> %UPGRADE_LOG_FILE%
	)


	echo ---------------------------------------------------

	echo --------------------------------------------------->> %UPGRADE_LOG_FILE%

	echo.
	echo Save log in cpe_firmware_upgrade.log
	echo.
	goto :EOF


rem ------------------------------------------------------------
rem  Check the status after start fw upgrading,
rem  Try to wait download done
rem  Usage:
rem    call :WAIT_DOWNLOAD_DONE IP Max_Wait_sec [not_first_time]
rem ------------------------------------------------------------
:WAIT_DOWNLOAD_DONE
	if %debug%==1 echo debug 1: in WAIT_DOWNLOAD_DONE IP=%1 remain_times=%2
	set /a remain_times=%2-1

	if %debug%==1 echo debug 1:snmpget -v 2c -c %SNMP_READ% %1 .1.3.6.1.4.1.99000.1.600.1.2.5.0
	snmpget -v 2c -c %SNMP_READ% %1 .1.3.6.1.4.1.99000.1.600.1.2.5.0 > tmp\tmp.log 2>&1

	for /f "delims=: tokens=3" %%k in (tmp\tmp.log) do (
		if not "%%k"=="" (
			if "%%k"==" 6" (
				if not "%3"=="not_first_time" (
					echo     %1 is downloading file ...
					echo     %1 is downloading file ... >> %UPGRADE_LOG_FILE%
				)

				rem check again if remain_times is not 0
				if not !remain_times!==0 (
					ping -n 1 127.0.0.1 > tmp\null.log
					if %debug%==1 ( echo debug 1: call :WAIT_DOWNLOAD_DONE %1 !remain_times!)
					call :WAIT_DOWNLOAD_DONE %1 !remain_times! not_first_time
				)else (

					if not "!still_download!"=="" (
						set still_download=!still_download!, %1
					) else (
						set still_download=%1
					)

					echo     %1 is still under downloading file ...
					echo     %1 is still under  downloading file ... >> %UPGRADE_LOG_FILE%
					echo     Don't power off the device 
					
					rem add uncomplete download unit to download list for later check
					echo %1 >> %DOWNLOAD_DONE_LIST_FILE%
				)
			) else if "%%k"==" 7" ( 
				set /a download_done_num=!download_done_num!+1
				
				if not "!download_done_list!"=="" (
					set download_done_list=!download_done_list!, %1
				) else (
					set download_done_list=%1
				)
				
				echo %1 >> %DOWNLOAD_DONE_LIST_FILE%

				if not "%2"=="" (
					set /a elapsed_secs=!MAX_CHECK_TIMES!-%2+1
				)
				
				echo     %1 download file done, elapsed !elapsed_secs! seconds
				echo     %1 is writing to flash... Don't power off the device  
				echo     %1 download file done, elapsed !elapsed_secs! seconds>> %UPGRADE_LOG_FILE%
				echo     %1 is writing file to flash... Don't power off the device>> %UPGRADE_LOG_FILE%
			) else if "%%k"==" 2" (
				echo     %1 upgrade action isn't beginning.
				echo     %1 upgrade action isn't beginning. >> %UPGRADE_LOG_FILE%

				rem check again if remain_times is not 0
				rem if not !remain_times!==0 (
				rem	if %debug%==1 ( echo debug 1: call :WAIT_DOWNLOAD_DONE %1 !remain_times!)
				rem	call :WAIT_DOWNLOAD_DONE %1 !remain_times!
				rem )
			) else if "%%k"==" 3" (
				set /a download_fail_num=!download_fail_num!+1

				if not "!download_fail_list!"=="" (
					set download_fail_list=!download_fail_list!, %1
				) else (
					set download_fail_list=%1
				)

				echo %1 >> %DOWNLOAD_FAIL_LIST_FILE%
			
				echo     %1 download file fail! 
				echo     %1 download file fail!  >> %UPGRADE_LOG_FILE%
			) else if "%%k"==" 4" (
				set /a invalid_fw_num=!invalid_fw_num!+1

				if not "!invalid_fw_list!"=="" (
					set invalid_fw_list=!invalid_fw_list!, %1
				) else (
					set invalid_fw_list=%1
				)

				echo %1 >> %INVALID_FW_LIST_FILE%
				
				echo     %1 firmware format isn't valid, Please check it. 
				echo     %1 firmware format isn't valid, Please check it. >> %UPGRADE_LOG_FILE%
			) else if "%%k"==" 1" (
				echo     %1 upgrade firmware successful 
				echo     %1 upgrade firmware successful >> %UPGRADE_LOG_FILE%
			) else if "%%k"==" 0" (
				echo     %1 is under busy. 
				echo     %1 is under busy. >> %UPGRADE_LOG_FILE%

				if not !remain_times!==0 (
					ping -n 1 127.0.0.1 > tmp\null.log
					if %debug%==1 ( echo debug 1: call :WAIT_DOWNLOAD_DONE %1 !remain_times!)
					call :WAIT_DOWNLOAD_DONE %1 !remain_times!
				)

			) else (
				echo     %1 other unknown status
				echo     %1 other unknown status >> %UPGRADE_LOG_FILE%
			)
		) else (
			echo Can't connect to %%i by SNMP!
		)
	)

:EOF

