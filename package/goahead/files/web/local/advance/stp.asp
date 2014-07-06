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

var stp = '<% getCfgZero(1,"stp_set"); %>';


function initTranslation(){
	var e = document.getElementById("stp_setup");
	e.innerHTML = _("stp setup");

	e = document.getElementById("stp_setup_title");
	e.innerHTML = _("stp");

	e = document.getElementById("STPEnable");
	e.innerHTML = _("stp set 1");

	e = document.getElementById("STPDisable");
	e.innerHTML = _("stp set 0");

	e = document.getElementById("stpApply");
	e.value = _("stp Apply");
	e = document.getElementById("stpReset");
	e.value = _("stp Reset");
}

function CheckValue()
{   
    showWebMessage(2, "");
    return true;
}

function initValue()
{
	initTranslation();
	if (1*stp== 1)
		document.stpCfg.stp_set[0].checked = true;
	else
		document.stpCfg.stp_set[1].checked = true; 
}

</script>
</head>

<body onLoad="initValue()">
<form method=post name="stpCfg" action="/goform/setStp" onSubmit="return CheckValue()">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li class="title" id="stp_setup">STP SetUp </li>
                <li class="w_text">
                  <table id=LanStpMode width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="15%" id="stp_setup_title">Spanning Tree Protocol (STP)</td>
                       <td><input type=radio id="stp_set_1" name=stp_set value="1"><font id="STPEnable">Enable</font>
                              <input type=radio id="stp_set_0" name=stp_set value="0" checked><font id="STPDisable">Disable</font>
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
        <input type=submit value="Apply" id="stpApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset value="Reset" id="stpReset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/stp.asp" name="stp_url">
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
