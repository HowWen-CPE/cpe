﻿<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<link href="/table.css" rel="stylesheet" type="text/css" />
<link href="/local/advance/images/expert.css" rel="stylesheet" type="text/css" />
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script>
Butterlate.setTextDomain("main");
port = "<% getCfgGeneral(1, "RemoteManagementPort"); %>"
lanip = "<% getCfgGeneral(1, "lan_ipaddr"); %>";


function refresh_info()
{
detect.location.href = "/checksysup.asp";
self.setTimeout('refresh_info()', 2000);
}
function process_lanip()
{
var current_url=location.href;
var current_url_len=current_url.length;
var flag=current_url.charAt(current_url_len-1);

if (flag==1)
detect.location.href = "/process_lanip.asp";
if (flag==2)
/*detect.location.href = "/process_dhcp.asp";*/
if (flag==3)
/*detect.location.href = "/process_dns.asp";*/
if (flag==4)
detect.location.href = "/process_snoop.asp";

self.setTimeout('refresh_info()', 2000);

}
setTimeout('process_lanip()', 3000);
function initTranslation()
{
	var e = document.getElementById("proceeding_message_1");
	e.innerHTML = _("proceeding message 1");
}
</script>	
</head>	
<body onLoad="initTranslation()">

<div id="popup5_table">
  <table width="600" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td width="19" height="37"><img src="images//popup_top_left.gif" width="19" height="45" /></td>
        <td background="images/popup_top_center.gif" class="table_title">&nbsp;</td>
        <td width="15" background="images/popup_top_center.gif">&nbsp;</td>
        <td width="19" height="45"><img src="images/popup_top_right.gif" width="19" height="45" /></td>
      </tr>
    </table></td>
  </tr>
  <tr>
    <td><table width="100%" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td width="19" background="images/popup_main_left.gif">&nbsp;</td>
        <td bgcolor="#FFFFFF"><span class="popupcontent"> 
          <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            
            <tr>
              <td height="28" colspan="3"><span class="space3"></span></td>
              </tr>
            <tr>
              <td width="31%"><center>
                <img src="/local/advance/images/loading.gif" width="83" height="83" />
              </center></td>
              <td width="69%" colspan="2"><span style="font-size:14px; font-weight:bold; color:#990000;"><a id="proceeding_message_1">Configuration Update is proceeding.  Please Wait ...</a></span></td>
              </tr>
            
            <tr>
              <td height="28" colspan="3"><span class="space3"></span></td>
            </tr>
            
            <tr>
              <td height="20" colspan="3">&nbsp;</td>
            </tr>
            <tr>
              <td height="20" colspan="3"><center>
              </center>              </td>
              </tr>
          </table>
         
        </span></td>
        <td width="19" background="images/popup_main_right.gif">&nbsp;</td>
      </tr>
    </table></td>
  </tr>
  <tr>
    <td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td width="19" height="48"><img src="images/popup_down_left.gif" width="19" height="48" /></td>
        <td background="images/popup_down_center.gif">&nbsp;</td>
        <td width="19" height="37"><img src="images/popup_down_right.gif" width="19" height="48" /></td>
      </tr>
    </table></td>
  </tr>
</table></div>
<iframe name='detect' src='' width='0' height='0' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'>             
</iframe>
</body>
</html>
