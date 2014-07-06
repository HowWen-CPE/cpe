<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

function CheckValue()
{ 
  showWebMessage(2, "");
  return true;
}

function myformCheck(){
  var form = document.rip;
  if(form.rip_enable[0].checked){
    document.rip.rip_version_1.value= document.rip.rip_version_1.checked ? "1" : "0";
    document.rip.rip_version_2.value = document.rip.rip_version_2.checked ? "1": "0";
    if(!form.rip_version_1.checked && !form.rip_version_2.checked) {
      alert("You must choose one rip version!");
      return false;
    }
    document.rip.RIPLan.value = document.rip.RIPLan.checked ? "1" : "0";
    document.rip.RIPWan.value = document.rip.RIPWan.checked ? "1": "0";
    if(!form.RIPLan.checked && !form.RIPWan.checked) {
      alert("Please choose one Interface in RIP!");
      return false;
    } 
  }
  if(form.ripng_enable[1].checked){
    showWebMessage(2, "");   
    return true;
  }
   document.rip.RIPngLan.value = document.rip.RIPngLan.checked ? "1" : "0";
   document.rip.RIPngWan.value = document.rip.RIPngWan.checked ? "1": "0";
   if(!form.RIPngLan.checked && !form.RIPngWan.checked) {
      alert("Please choose one Interface in RIPng!");
      return false;
   }
    showWebMessage(2, "");   
    return true;
}

function initValue()
{
  var rip_enable = "<% getCfgGeneral(1, "RIP_enable"); %>";
  var ripng_enable = "<% getCfgGeneral(1, "RIPng_enable"); %>";
  var rip_version_v1 = "<% getCfgGeneral(1,"RIP_version_v1");%>";
  var rip_version_v2 = "<% getCfgGeneral(1,"RIP_version_v2");%>";
  var rip_lan = "<% getCfgGeneral(1,"RIP_lan");%>";
  var rip_wan = "<% getCfgGeneral(1,"RIP_wan");%>";
  var ripng_lan = "<% getCfgGeneral(1,"RIPng_lan");%>";
  var ripng_wan = "<% getCfgGeneral(1,"RIPng_wan");%>";
  if(rip_enable == 1) {
  document.rip.rip_enable[0].checked = true;
  }else
  document.rip.rip_enable[1].checked = true;
  if(ripng_enable == 1) {
  document.rip.ripng_enable[0].checked = true;
  }else
  document.rip.ripng_enable[1].checked = true;
  if(rip_version_v1 == 1)
  document.rip.rip_version_1.checked = true;
  if(rip_version_v2 == 1)
  document.rip.rip_version_2.checked = true;
  if(rip_lan == 1)
  document.rip.RIPLan.checked = true;
  if(rip_wan == 1)
  document.rip.RIPWan.checked = true;
  if(ripng_lan == 1)
  document.rip.RIPngLan.checked = true;
  if(ripng_wan == 1)
  document.rip.RIPngWan.checked = true;
  ripRadioChange();
  ripngRadioChange();
  return ;
}
function ripRadioChange(){
  var form=document.rip;
  if(form.rip_enable[0].checked) {
  form.rip_version_1.disabled=false;
  form.rip_version_2.disabled=false;
  form.RIPLan.disabled=false;
  form.RIPWan.disabled=false;
  //VersionCheck();
  }else {
  form.rip_version_1.disabled=true;
  form.rip_version_2.disabled=true;
  form.RIPLan.disabled=true;
  form.RIPWan.disabled=true;
  }
  return true;
}
function ripngRadioChange(){
  var form=document.rip;
  if(form.ripng_enable[0].checked) {  
  form.RIPngLan.disabled=false;
  form.RIPngWan.disabled=false;
  //VersionCheck();
  }else {
  form.RIPngLan.disabled=true;
  form.RIPngWan.disabled=true;
  }
  return true;
}
function VersionCheck()
{
    var form=document.rip;
    if(!form.rip_version_1.checked && !form.rip_version_2.checked ) {
      form.RIPLan.disabled=true;
      form.RIPWan.disabled=true;
    }else {
      form.RIPLan.disabled=false;
      form.RIPWan.disabled=false;
    }
}
function ngCheck()
{
    var form=document.rip;
    if(form.ripng_enable.checked == true) {                                                                                                        
    }else {                                                                   
                                                                            
    }
}
</script>
</head>

<body onLoad="initValue()">
<form method=post name="rip" action="/goform/ripRouting" onSubmit="return myformCheck()">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li class="title" id="riptitle">RIP</li>
                 <li class="w_text">
                    <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                      <tr>
                        <td width="40%" id="RemoteManagementServerPort">RIP : </td>
                        <td width="5%"><input name="rip_enable" onclick="ripRadioChange()" value="1" type="radio" />&nbsp;&nbsp;&nbsp;Enable </td>
                        <td width="5%" id="RIPEnable"></td>
                        <td width="5%"><input name="rip_enable" onclick="ripRadioChange()" value="0" type="radio" />&nbsp;&nbsp;&nbsp;Disable </td>
                        <td width="5%" id="RIPDisable"></td>
                        <td width="35%" > </td>
                      </tr>
                    </table>
                 </li>
                  <li class="w_text">
                    <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                      <tr>
                        <td width="40%" id="RemoteManagementServerPort">RIP Version Support : </td>
                        <td width="5%"><input name="rip_version_1" value="1" type="checkbox" onClick="VersionCheck()" />&nbsp;&nbsp;&nbsp;RIPv1</td>
                        <td width="5%" id="RIPEnableV1"></td>
                        <td width="5%"><input name="rip_version_2" value="1" type="checkbox" onClick="VersionCheck()" />&nbsp;&nbsp;&nbsp;RIPv2 </td>
                        <td width="5%" id="RIPEnableV2"></td>
                        <td width="35%" > </td>
                      </tr>
                    </table>
                  </li>
                  <li class="w_text">
                    <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                      <tr>
                        <td width="40%" id="RemoteManagementServerPort">Interface : </td>
                        <td width="5%"><input name="RIPLan" value="1" type="checkbox" onClick="VersionCheck()" />&nbsp;&nbsp;&nbsp;LAN</td>
                        <td width="5%" id="RIPLan"></td>
                        <td width="5%"><input name="RIPWan" value="1" type="checkbox" onClick="VersionCheck()" />&nbsp;&nbsp;&nbsp;WAN</td>
                        <td width="5%" id="RIPWan"></td>
                        <td width="35%" > </td>
                      </tr>
                    </table>
                  </li>                    
                  <li class="space3"></li>
                  <li class="title" id="riptitle">RIPng</li>
                  <li class="w_text">
                    <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                      <tr>
                        <td width="40%" id="RemoteManagementServerPort">RIPng : </td>
                        <td width="5%"><input name="ripng_enable" onclick="ripngRadioChange()" value="1" type="radio" />&nbsp;&nbsp;&nbsp;Enable</td>
                        <td width="5%" id="RIPEnable"></td>
                        <td width="5%"><input name="ripng_enable" onclick="ripngRadioChange()" value="0" type="radio" />&nbsp;&nbsp;&nbsp;Disable</td>
                        <td width="5%" id="RIPDisable"></td>
                        <td width="35%" > </td>
                      </tr>
                    </table>
                 </li>                 
                  <li class="w_text">
                    <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                      <tr>
                        <td width="40%" id="RemoteManagementServerPort">Interface : </td>
                        <td width="5%"><input name="RIPngLan" value="1" type="checkbox" onClick="ngCheck()" />&nbsp;&nbsp;&nbsp;LAN</td>
                        <td width="5%" id="RIPngLan"></td>
                        <td width="5%"><input name="RIPngWan" value="1" type="checkbox" onClick="ngCheck()" />&nbsp;&nbsp;&nbsp;WAN</td>
                        <td width="5%" id="RIPngWan"></td>
                        <td width="35%" > </td>
                      </tr>
                    </table>
                  </li>   
        <li></li>
        <li></li>
              </ul>
            </div>
        </li>
        <center>
        <li class="table_button"> 
        <input type=submit value="Apply" id="ripApply" name="add" onSubmit="return myformCheck()">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset value="Cancel" id="ripReset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/rip.asp" name="rip_url">
        </li>
        </center>
</form>
</body>
<script>
function alert(str)
{
  showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script> 
</html>
