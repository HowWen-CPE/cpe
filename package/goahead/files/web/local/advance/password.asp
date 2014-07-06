<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>
</head>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/JavaScript">
Butterlate.setTextDomain("main");

function  allTrim(ui){  
    var  notValid=/\s/;
	  
    while(notValid.test(ui)){  
        ui=ui.replace(notValid,"");
	} 
	 
    return ui;
}

function isASCII(str)
{
	for( var i = 0; i < str.length; i++)
	{
		 var chr = str.charCodeAt(i);
		 if( chr < 0 || chr > 255 )
		 {
		    return false;
		 }
	}
	
	return true;
}

function checkInput(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) == '^' ) || (str.charAt(i) == '|' )  || (str.charAt(i) == '%' )  || (str.charAt(i) == '$' )  || (str.charAt(i) == ';' ))
		return 1;
	}
	return 0;
}

function check_confirm()
{
	if (document.Lang.User_Select.selectedIndex == 0) {
		if(document.forms[0].admpass_old.value < 1 || document.forms[0].admpass.value < 1) {
				alert("Password not NULL");
				document.forms[0].admpass_old.focus();
				return false;		
		} else {
			  admpass_old = document.forms[0].admpass_old.value;
				tmp_admpass_old = allTrim(admpass_old);

				if (! isASCII(tmp_admpass_old)) {
						alert("invalid old password: cann't include chinese!");
						document.forms[0].admpass_old.focus();
						return false;
				}	
				
				admpass = document.forms[0].admpass.value;	
			  tmp_admpass = allTrim(admpass);
			  
			  if ( ! isASCII(tmp_admpass)) {
						alert("invalid new password: cann't include chinese!");
						document.forms[0].admpass.focus();
						return false;
				}	
		}
	} else {
		if(document.forms[0].admpass.value < 1) {
				alert("Password not NULL");
				document.forms[0].admpass.focus();
				return false;	
		} else {
			  admpass = document.forms[0].admpass.value;	
			  tmp_admpass = allTrim(admpass);
			  
			  if ( ! isASCII(tmp_admpass)) {
						alert("invalid new password: cann't include chinese!");
						document.forms[0].admpass.focus();
						return false;
				}	
		}
	}
		
    if(checkInput(document.forms[0].admpass.value)==1){
    	alert("System do not support specific invalid characters(^ | % $ ;)");
		return false;
    }
    
	if (document.forms[0].admpass.value!=document.forms[0].admpass_confirm.value)
	{
		alert("Error: Confirm password not match!!");
		return false;
	}
	else
	{
		showWebMessage(2, "");
		document.forms[0].submit();
		return true;
	}
}

function initTranslation()
{
	var e;

	e = document.getElementById("pwTitle");
	e.innerHTML = _("pw title");
	e = document.getElementById("user_name");
	e.innerHTML = _("pw username");
	e = document.getElementById("pwOldPw");
	e.innerHTML = _("pw old pw");
	e = document.getElementById("pwNewPw");
	e.innerHTML = _("pw new pw");
	e = document.getElementById("pwConfirm");
	e.innerHTML = _("pw confirm");

	e = document.getElementById("pwApply");
	e.value = _("pw apply");
	e = document.getElementById("pwReset");
	e.value = _("pw reset");
}

function Userchange()
{
	if(document.Lang.User_Select.selectedIndex == 0)
	{
		document.getElementById("admpass_old_1").style.visibility= "visible";
	}
	else
	{
		document.getElementById("admpass_old_1").style.visibility= "hidden";
	}
}
</script>
<body onload="initTranslation();">
<form method="post" name="Lang" action="/goform/setSysPass">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li class="title" id="pwTitle">Password Setup </li>
				<li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><font id="user_name">User Name</font> : </td>
    					<td>
						<select name="User_Select" size="1" onchange="Userchange()">
						<option value="admin">admin</option>
						<option value="guest">guest</option>
						</select>
						</td>
                    </tr>
                  </table>
                </li>
                <li id="admpass_old_1" class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><font id="pwOldPw">Old Password</font> : </td>
    									<td><input type="password" name="admpass_old" size="30" maxlength="30" value=""></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><font id="pwNewPw">New Password</font> : </td>
    									<td><input type="password" name="admpass" size="30" maxlength="30" value=""></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><font id="pwConfirm">Retype to Confirm</font> : </td>
    									<td><input type="password" name="admpass_confirm" size="30" maxlength="30" value=""></td>
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
        <input type="button"  value="Apply" id="pwApply"  onclick="return check_confirm();"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type="reset" 	value="Reset" id="pwReset"  >
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
