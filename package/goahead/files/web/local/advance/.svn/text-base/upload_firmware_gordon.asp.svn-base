<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("admin");

var zyfw_size="";
var n=1;

function initTranslation()
{
	var e = document.getElementById("uploadTitle");
	e.innerHTML = _("upload title");
	e = document.getElementById("uploadIntroduction1");
	e.innerHTML = _("upload introduction1");
	e = document.getElementById("uploadTitleOnLine");
	e.innerHTML = _("upload titleOnLine");
	
	e = document.getElementById("uploadCurrentFwVer");
	e.innerHTML = _("upload current FW version");
	e = document.getElementById("uploadLatestFwVer");
	e.innerHTML = _("upload latest FW version");
	e = document.getElementById("uploadReleaseDate");
	e.innerHTML = _("upload release date");
	e = document.getElementById("uploadReleaseNote");
	e.innerHTML = _("upload release note");
	e = document.getElementById("uploadSize");
	e.innerHTML = _("upload size");


	e = document.getElementById("uploadFWPath");
	e.innerHTML = _("upload Path");
	e = document.getElementById("uploadFWUpload");
	e.value = _("admin Upload");
	e = document.getElementById("Check_for_Latest_Firmware_Now");
	e.value = _("check latest Upload");
	
	e = document.getElementById("Do_Firmware_Upgrade");
	e.value = _("do FW upgrade");

}

function show_upload_process()
{
		//parent.location.href="/local/advance/upload_process.html";
		self.location.href="/local/advance/checkupload.asp";
		
}
var http_request = false;
function makeRequest(url, content, handler) {
	http_request = false;
	if (window.XMLHttpRequest) { // Mozilla, Safari,...
		http_request = new XMLHttpRequest();
		if (http_request.overrideMimeType) {
			http_request.overrideMimeType('text/xml');
		}
	} else if (window.ActiveXObject) { // IE
		try {
			http_request = new ActiveXObject("Msxml2.XMLHTTP");
		} catch (e) {
			try {
			http_request = new ActiveXObject("Microsoft.XMLHTTP");
			} catch (e) {}
		}
	}
	if (!http_request) {
		alert('Giving up :( Cannot create an XMLHTTP instance');
		return false;
	}
	http_request.onreadystatechange = handler;
	http_request.open('POST', url, true);
	http_request.send(content);
}

function FWHandler() {
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			var flag=http_request.responseText;
			if(n<=9)
			{
				if(flag == "1"){
					top.location.href = "/local/advance/upload_process.asp";
				}
				else if(flag == "0")
				{
					n++;
					window.setTimeout('getvalue()', 5000);
				}
				else{
					alert(_('firmware transport failure'));
					window.location.href = '/local/advance/upload_firmware_gordon.asp';
				}
			}
			else
			{

				alert(_('firmware transport timeout'));
				window.location.href = '/local/advance/upload_firmware_gordon.asp';
			}
		} else {
			alert('There was a problem with the request.');
		}
	}
}
function getvalue()
{
	makeRequest("/goform/getGetUploadFlag", "n/a", FWHandler);
	
}
function jump()
{
	window.location.href = "/local/advance/upload_process.asp";
}
function uploadFirmwareCheck()
{

	var j ="<% MatchIpAndMac(); %>";
	if(j=="2")
	{
		alert(_("Please upgrade your firmware with ethernet"));
		return false;	
	}
	if(j=="-1")
	{
		alert("cannot ping well, check your network");
		self.location.href="/local/advance/upload_firmware_gordon.asp";
	}
	if(j=="0")
	{
		alert("Temporary file can not open, check your bridge setting");
		self.location.href="/local/advance/upload_firmware_gordon.asp";
	}
	if(document.UploadFirmware.filename.value == ""){
		alert("Firmware Upgrade: Please specify a file.");
		return false;
	}
	var FILENAME_LEN = document.UploadFirmware.filename.value.length;
	if (document.UploadFirmware.filename.value.substring(FILENAME_LEN-4,FILENAME_LEN)!=".bin")
	{
			alert(_("Not a valid firmware."));
			return false;
	}
	if (!confirm(_("Do you wish to continue with the upgrade process ?")))
		return false;
  showWebMessage(2, "");
  self.setTimeout('getvalue()', 5000);
}

function Link2ReleaseNote() 
{
//window.open("/release_note.pdf");
//var url_str="ftp://zyus2504:zyus2504@ftp2.HP.com.tw/WP777/firmware/"+parent.zyfw_release_note;
var url_str="ftp://ftp2.HP.com/WP777/firmware/"+parent.zyfw_release_note;
window.open(url_str);
}	

function show_upload_process_auto()
{
	detect.location.href="/upload_process_auto.asp";
  self.setTimeout('show_upload_process_auto()', 2000);
}

function kill_wget()
{
	detect.location.href="/kill_wget.asp";
}	

function do_uploadFirmwareCheck()
{
	if (!confirm(_("Do you wish to continue with the upgrade process ?")))
		return false;
  showWebMessage(0, "Firmware download in progress. Please wait . . .");
  self.setTimeout('show_upload_process_auto()', 5000);
  //self.setTimeout('kill_wget()', 60000);
}

</script>	
</head>
<body onLoad="initTranslation()">
<form method="post" name="UploadFirmware" action="/cgi-bin/upload.cgi" enctype="multipart/form-data">
    <div id="table">
      <ul>
          <li class="table_content">
            <div class="data">
              <ul>
                <li class="title" id="uploadTitle">Firmware Upgrade </li>
                <li class="w_text2">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td height="40" id="uploadIntroduction1">To upgrade the internal device firmware, browse to the location of the binary (.BIN) upgrade file and click Upload. Upgrade files can be downloaded from website. If the upgrade file is compressed (.ZIP file), you must first extract the binary (.BIN) file. In some cases, you may need to reconfigure. </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="10%" id="uploadFWPath" nowrap>File Path:</td>
                      <td><input name="filename" size="20" maxlength="128" type="file"></td>	
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                </li>

                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><input value="Upload" id="uploadFWUpload" name="UploadFirmwareSubmit" type="submit" onClick="return uploadFirmwareCheck();"></td>
                    </tr>
                  </table>
                </li>
              </ul>
            </div>
        </li>
        </ul>
    </div>
</form>

<form method="post" name="UploadFirmware2" action="/cgi-bin/upload2.cgi" enctype="multipart/form-data">
    <div id="table_UploadFirmware2" style="visibility:hidden;display:none">
      <ul>
          <li class="table_content">
            <div class="data">
              <ul>
				<li class="space3"></li>
                <li class="title" id="uploadTitleOnLine">On-line Firmware Upgrade </li>
                <!-- <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><input  name="time_ServerType" value="1" onclick="timeserver_enable_switch()" type="radio" /><font id="UserDefined"></font></td>
                      <td><input  name="NTPServerIP" size="20" maxlength="31" value="ftp://ftp2.HP.com/WR71210IRA/firmware" type="text" /></td>
                    </tr>
                  </table>
                </li>^M -->
                <li class="w_text2">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><input value="Check for Latest Firmware Now" id="Check_for_Latest_Firmware_Now" name="Check_for_Latest_Firmware_Now" type="submit" onclick="showWebMessage(0, 'Retrieve new firmware information now. Please wait . . .');"></td>
                    </tr>
                  </table>
                </li>
              </ul>
            </div>
        </li>
        </ul>
    </div>
</form>
<form method="post" name="UploadFirmware3" action="/cgi-bin/upload3.cgi" enctype="multipart/form-data">
    <div id="table_UploadFirmware3" style="visibility:hidden;display:none">
      <ul>
          <li class="table_content">
            <div class="data">
              <ul>
                <li class="w_text2">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td height="40"><font id="uploadCurrentFwVer">You are currently using firmware version:</font><% getFirmwareVersion(); %><% getFirmwareDate(); %></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="10%"><span class="top_left"><font id ="uploadLatestFwVer">The Latest Firmware Version</font></span></td>
                      <td width="10%"><span class="top_font"><font id =""><script>document.write(parent.zyfw_fw_version);</script></font></span></td>
                    </tr>
                    <tr>
                      <td width="10%"><span class="top_left"><font id ="uploadReleaseDate">Release Date</font></span></td>
                      <td width="10%"><span class="top_font"><font id =""><script>document.write(parent.zyfw_release_date);</script></font></span></td></tr>
                    <tr>
                      <td width="10%"><span class="top_left"><font id ="uploadReleaseNote">Release Note</font></span></td>
                      <td width="10%"><span class="top_font"><font id =""><a href="javascript: Link2ReleaseNote();">Click here to open</a></font></span></td></tr>
                    <tr>
                      <td width="10%"><span class="top_left"><font id ="uploadSize">Size</font></span></td>
                      <td width="10%"><span class="top_font"><font id =""><script>document.write(parent.zyfw_size); zyfw_size=parent.zyfw_size;</script></font></span></td>
                    </tr>
                  </table>
                  <br>
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><input value="Do Firmware Upgrade" id="Do_Firmware_Upgrade" name="Do_Firmware_Upgrade" type="submit" onClick="return do_uploadFirmwareCheck();"></td>
                    </tr>
                  </table>
                </li>
              </ul>
            </div>
        </li>
        </ul>
    </div>
</form>
<br>
<iframe name='detect' src='/temp.html' width='0' height='0' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'>             
</iframe>
    <div>
      <ul>
        <li class="table_content">
                    
          <li class="table_button">
          </li>
        </li>
        </ul>
    </div>
</body>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
if (parent.upload_message=="Ready")
	showWebMessage(0, parent.upload_message);
else
	showWebMessage(1, parent.upload_message);
parent.upload_message="Ready";
if ((parent.zyfw_fw_version!='') && (parent.zyfw_fw_version!='<% getFirmwareVersion(); %>'))
{
	document.getElementById("table_UploadFirmware3").style.visibility = "visible";
	document.getElementById("table_UploadFirmware3").style.display = "block";
}
if ((parent.zyfw_fw_version!='') && (parent.zyfw_fw_version=='<% getFirmwareVersion(); %>'))
{
	showWebMessage(0, "No newer firmware exists. The firmware on your device is already the latest version.");
}
parent.zyfw_fw_file="";
parent.zyfw_fw_version="";
parent.zyfw_release_date="";
parent.zyfw_size="";
</script> 
</html>
