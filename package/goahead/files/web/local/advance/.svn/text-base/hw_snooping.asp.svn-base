<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
.on {display:on}
.off {display:none}
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

var snooping = '<% getCfgZero(1,"snooping_set"); %>';


function initTranslation(){
	var e = document.getElementById("IP_Address");
	e.innerHTML = _("ipAlias Address");
	e = document.getElementById("IP_Subnet_Mask");
	e.innerHTML = _("ipAlias Subnet");
	e = document.getElementById("lanApply");
	e.value = _("ipAlias Apply");
	e = document.getElementById("lanReset");
	e.value = _("ipAlias Reset");
}

function CheckValue()
{   
   
    showWebMessage(2, "");
    return true;
}

function initValue()
{
     //initTranslation();
   if (1*snooping== 1)
	document.snoopingCfg.snooping_set[0].checked = true;
  else
       document.snoopingCfg.snooping_set[1].checked = true; 
}

</script>
</head>

<body onLoad="initValue()">
<form method=post name="snoopingCfg" action="/goform/setSnooping" onSubmit="return CheckValue()">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li class="title">IGMP</li>
                <li class="w_text">
                  <table id=LanStpMode width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%">IGMP snooping :</td>
                       <td><input type=radio id=igmp_set_1 name=snooping_set value="1"><font id="IGMPEnable">Enable</font>
                              <input type=radio id=igmp_set_0 name=snooping_set value="0" checked><font id="IGMPDisable">Disable</font>
                        </td>
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
        <input type=submit value="Apply" id="IGMPApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset value="Reset" id="IGMPReset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/hw_snooping.asp" name="snooping_url">
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
