<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("admin");

function initTranslation()
{
	var board_model = "<% getCfgZero(1, "board_model"); %>";
	e = document.getElementById("title_2");
	e.innerHTML = _("syssettings title_2");
	e = document.getElementById("introduction_2");
	e.innerHTML = _("syssettings introduction_2");
	e = document.getElementById("introduction_3");
	e.innerHTML = _("syssettings introduction_3");
	e = document.getElementById("introduction_4");
	e.innerHTML = _(board_model+"_"+"syssettings introduction_4");
	e = document.getElementById("introduction_6");
	e.innerHTML = _("syssettings introduction_6");
	e = document.getElementById("reset");
	e.value = _("syssettings title_2");
}

function LoadDefaultFormCheck()
{
	if (!confirm(_("Do you wish to continue with this process ?")))
		return false;
	return true;
}

</script>
</head>
<body onLoad="initTranslation()">
<form method="post" name="LoadDefaultSettings" action="/goform/LoadDefaultSettings">
    <div id="table">
      <ul>
          <li class="table_content">
            <div class="data">
              <ul>
                <li id="title_2" class="title">Back to Factory Defaults </li>

                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="introduction_2">
                        <span style="font-weight: 400;">
                          Click <b>Reset</b> to clear all user-entered configuration information and return to factory defaults. After resetting, the 
                        </span>
                      </td>
                    </tr>
                  </table>
                </li>

                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="introduction_3"><span style="font-weight: 400;">- Password will be admin </span></td>
                    </tr>
                  </table>
                </li>

                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="introduction_4">
                        <span  style="font-weight: 400;">
                          - LAN IP address will be 192.168.1.2 
                        </span>
                      </td>
                    </tr>
                  </table>
                </li>

                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="introduction_6">
                        <span  style="font-weight: 400;">
                          - Operation mode will be reset to AP Mode 
                        </span>
                      </td>
                    </tr>
                  </table>
                </li>

              </ul>

			  <ul>
			    <li></li>
			  </ul>

            </div>
          </li>
     </ul>

	<ul><center>
	  <li class="table_button">
		<input name="LoadDefault" value="Reset"  id="reset" type="submit" onClick="return LoadDefaultFormCheck()" >&nbsp;
	  </li></center>
	</ul>
	</div>
</form>
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
<!--showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));-->
</script> 
</html>
