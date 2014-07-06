<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<meta http-equiv="Pragma" content="no-cache">
<head>
<link href="images/css.css" rel="stylesheet" type="text/css" />
<link href="advance/images/table.css" rel="stylesheet" type="text/css" />
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" /></head>
<style>
.on {display:on}
.off {display:none}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("main");

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
	else
		document.getElementById(id).className  = "off" ;
}

function atoi(str, num)
{
	i = 1;
	if (num != 1) {
		while (i != num && str.length != 0) {
			if (str.charAt(0) == '.') {
				i++;
			}
			str = str.substring(1);
		}
		if (i != num)
			return -1;
	}

	for (i=0; i<str.length; i++) {
		if (str.charAt(i) == '.') {
			str = str.substring(0, i);
			break;
		}
	}
	if (str.length == 0)
		return -1;
	return parseInt(str, 10);
}

function checkRange(str, num, min, max)
{
       var k = 0;
	 for (var i=0; i<str.length; i++) {
		if (str.charAt(i) == '.')
			k = k+1;
		continue;
	}
	if(k > 3){
	      alert("Error. IP address is not valid.");
             return false;
	}
	d = atoi(str, num);
	if (d > max || d < min)
		return false;
	return true;
}

function isAllNum(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
}

function checkIpAddr(field, ismask)
{
	if (field.value == "") {
		alert(_("Error. IP address is empty."));
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	if (isAllNum(field.value) == 0) {
		alert(_('It should be a [0-9] number.'));
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
    /*
	if (ismask) {
		if ((!checkRange(field.value, 1, 0, 256)) ||
				(!checkRange(field.value, 2, 0, 256)) ||
				(!checkRange(field.value, 3, 0, 256)) ||
				(!checkRange(field.value, 4, 0, 256)))
		{
			alert(_('IP adress format error.'));
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	*/
	//aron modify to limit max to 255
	if (ismask) {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 0, 255)))
		{
			alert(_('IP adress format error.'));
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	else {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 1, 254)))
		{
			alert(_('IP adress format error.'));
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	return true;
}

function CheckValue()
{
		 if (document.wanCfg.l2tpMode[1].checked == true) {
			if (!checkIpAddr(document.wanCfg.l2tpIp, false))
				return false;
			if (!checkIpAddr(document.wanCfg.l2tpNetmask, true))
				return false;
			if (!checkIpAddr(document.wanCfg.l2tpGateway, false))
				return false;
		 }
		 
	if (!checkIpAddr(document.wanCfg.l2tpServer, false))
			return false;
			
  if(document.wanCfg.l2tpUser.value == "")
  {
    alert(_("L2TP user name should not be empty!"));
		return false;
  }
  if(document.wanCfg.l2tpPass.value == "")
  {
    alert(_("L2TP password should not be empty!"));
		return false;
  }
  return true;
}

function l2tp_mode_switch()
{
    if (document.wanCfg.l2tpMode[0].checked == true)
	{		
		document.wanCfg.l2tpGateway.disabled = true;
						show_div(false, "div_l2tpGateway");
		document.wanCfg.l2tpNetmask.disabled = true;
						show_div(false, "div_l2tpNetmask");
		document.wanCfg.l2tpIp.disabled = true;
						show_div(false, "div_l2tpIp");
	}
	else
	{
		document.wanCfg.l2tpGateway.disabled = false;
						show_div(true, "div_l2tpGateway");
		document.wanCfg.l2tpNetmask.disabled = false;
						show_div(true, "div_l2tpNetmask");
		document.wanCfg.l2tpIp.disabled = false;
						show_div(true, "div_l2tpIp");
	}
}

function onChangeType(item)
{
	if (item.selectedIndex==0)
		self.location.href="genie2-2.asp";
	if (item.selectedIndex==1)
		self.location.href="genie2-3.html";
	if (item.selectedIndex==2)
		self.location.href="genie2-1.asp";
	if (item.selectedIndex==3)
		self.location.href="genie2-4.asp";
	if (item.selectedIndex==4)
		self.location.href="genie2-5.asp";
}

function MM_findObj(n, d) { //v4.01
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && d.getElementById) x=d.getElementById(n); return x;
}

function MM_showHideLayers() { //v6.0
  var i,p,v,obj,args=MM_showHideLayers.arguments;
  for (i=0; i<(args.length-2); i+=3) if ((obj=MM_findObj(args[i]))!=null) { v=args[i+2];
    if (obj.style) { obj=obj.style; v=(v=='show')?'visible':(v=='hide')?'hidden':v; }
    obj.visibility=v; }
}
function initTranslation()
{
	var e = document.getElementById("g2-2easy123");
	e.innerHTML = _("g2-2 easy123");
		e = document.getElementById("g2-2Internet");
	e.innerHTML = _("g2-2 Internet");
		e = document.getElementById("g2-2Password");
	e.innerHTML = _("g2-2 Password");
		e = document.getElementById("g2-2Security");
	e.innerHTML = _("g2-2 Security");
		e = document.getElementById("g2-2InternetType");
	e.innerHTML = _("g2-2 InternetType");
		e = document.getElementById("g2-2InternetType1");
	e.innerHTML = _("g2-2 InternetType1");
		e = document.getElementById("g2-2InternetType2");
	e.innerHTML = _("g2-2 InternetType2");
		e = document.getElementById("g2-2InternetType3");
	e.innerHTML = _("g2-2 InternetType3");
		e = document.getElementById("g2-2InternetType4");
	e.innerHTML = _("g2-2 InternetType4");
		e = document.getElementById("g2-2InternetType5");
	e.innerHTML = _("g2-2 InternetType5");
		e = document.getElementById("g2-2title");
	e.innerHTML = _("g2-2 title");
			e = document.getElementById("g2-5Dynamic");
	e.innerHTML = _("g2-5 Dynamic");
			e = document.getElementById("g2-5Static");
	e.innerHTML = _("g2-5 Static");
			e = document.getElementById("g2-5Address");
	e.innerHTML = _("g2-5 Address");
			e = document.getElementById("g2-5Subnet");
	e.innerHTML = _("g2-5 Subnet");
			e = document.getElementById("g2-5Gateway");
	e.innerHTML = _("g2-5 Gateway");
	e = document.getElementById("g2-5Server");
	e.innerHTML = _("g2-5 Server");
	e = document.getElementById("g2-5User");
	e.innerHTML = _("g2-5 User");
	e = document.getElementById("g2-5Password");
	e.innerHTML = _("g2-5 Password");

		
		
		
			e = document.getElementById("g2-2Next");
	e.innerHTML = _("g2-2 Next");
			e = document.getElementById("g2-2Back");
	e.innerHTML = _("g2-2 Back");
			e = document.getElementById("g2-2Exit");
	e.innerHTML = _("g2-2 Exit");
			e = document.getElementById("g2-2No");
	e.innerHTML = _("g2-2 No");
	e = document.getElementById("g2-2Yes");
	e.innerHTML = _("g2-2 Yes");
		e = document.getElementById("g2-2msg");
	e.innerHTML = _("g2-2 msg");
		e = document.getElementById("log_out");
	e.innerHTML = _("dash logout title");
}
</script>
<body onLoad="initTranslation()">
<form method=post name="wanCfg" action="/goform/setWan">
<div id="popup"></div>
<div id="wrapper">
<div id="close"><a href="#"onclick="MM_showHideLayers('popup3_table','','show','popup','','show')" title="Exit"></a></div>
<div class="unit_eazy"></div>
<div class="unit_title" id="g2-2easy123">eaSetup 123</div>
<div class="genie_logo"></div>
<div class="genie_color"></div>
<div class="set"><ul><li class="g_ing" id="g2-2Internet">Connect to Internet </li> <li class="g_arrow"></li><li class="g_default" id="g2-2Password">Router Password </li> <li class="g_arrow"></li><li class="g_default" id="g2-2Security"> Wireless Security</li></ul></div>
<div class="set_now"><ul><li>1</li></ul></div>
<div class="set_no4"><ul> <li>2</li> </ul></div>
<div class="set_no5"><ul><li>3</li></ul></div>
<div class="g_word">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="235" class="b_txt" id="g2-2InternetType">Internet Connection Type:</td>
<td width="425"><label>
<select name="menu1" class="select1" onchange="onChangeType(this);">
<option id="g2-2InternetType1">Static IP</option>
<option id="g2-2InternetType2">DHCP</option>
<option id="g2-2InternetType3">PPPoE</option>
<option id="g2-2InternetType4">PPTP</option>
<option id="g2-2InternetType5"selected="selected">L2TP</option>
</select>
</label></td>
</tr>
<tr>
<td colspan="2" id="g2-2title">Please refer to the information provided by your Internet Service Provider (ISP) and complete the following blanks.</td>
</tr>
</table>
</div>
<div class="input_bn2">
<ul>
<li>
<table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="230"><input name="l2tpMode" type="radio"  value="1" onclick="l2tp_mode_switch()" checked /><a id=g2-5Dynamic>Dynamic IP</a>
<input name="l2tpMode" type="radio" value="0" onclick="l2tp_mode_switch()" /><a id=g2-5Static>Static IP</a></td>
<td>&nbsp;</td>
</tr>
<tr id=div_l2tpIp>
<td width="230" id="g2-5Address">L2TP Address:</td>
<td width="200"><input name="l2tpIp" class="input1" maxlength=15 size="25" value="<% getCfgGeneral(1, "wan_l2tp_ip"); %>"></td>
<td>&nbsp;</td>
</tr>
<tr id=div_l2tpNetmask>
<td width="230" id="g2-5Subnet">L2TP Subnet Mask:</td>
<td width="200"><input name="l2tpNetmask" class="input1" maxlength=15 size="25" value="<% getCfgGeneral(1, "wan_l2tp_netmask"); %>"></td>
<td>&nbsp;</td>
</tr>
<tr id=div_l2tpGateway>
<td width="230" id="g2-5Gateway">L2TP Gateway IP Address:</td>
<td width="200"><input name="l2tpGateway" class="input1" maxlength=15 size="25" value="<% getCfgGeneral(1, "wan_l2tp_gateway"); %>"></td>
<td>&nbsp;</td>
</tr>
<tr>
<td width="230" id="g2-5Server">L2TP Server IP Address:</td>
<td width="200"><input name="l2tpServer" class="input1" maxlength=15 size="25" value="<% getCfgGeneral(1, "wan_l2tp_server"); %>"></td>
<td>&nbsp;</td>
</tr>
<tr>
<td width="230" id="g2-5User">User Name:</td>
<td width="200"><input name="l2tpUser"  class="input1" type="text" size="25" value="<% getCfgGeneral(1, "wan_l2tp_user"); %>" /></td>
<td>&nbsp;</td>
</tr>
<tr>
<td width="230" id="g2-5Password">Password:</td>
<td width="200"><input name="l2tpPass"  class="input1" type="text" size="25" value="<% getCfgGeneral(1, "wan_l2tp_pass"); %>" /></td>
<td>&nbsp;</td>
</tr>
</table>
</li>
</ul>
</div>
<div class="button2"><ul><li>
<input name="connectionType" type="hidden" value="L2TP">
<input name="l2tpNailup" type="hidden" value="1">
<input name="dns1Type" type="hidden" value="ISP">
<input name="dns2Type" type="hidden" value="ISP">
<input type=hidden value="/local/genie2-4.html" name="wan_url">
<a id="g2-2Next" href="javascript: if (CheckValue()) document.wanCfg.submit();">Next</a></li>
<li><a  id="g2-2Back" href="genie.asp">Back</a> 
<li><a  id="g2-2Exit" href="#" onclick="MM_showHideLayers('popup','','show','popup3_table','','show')" title="Exit" style="margin-right:20px;">Exit</a></li></ul></div> 
<div class="index_bg" id="index_bg"></div>
<div class="in_line" id="topline" ></div>
<div class="frame_left"></div>
<div class="frame_right"></div>
<div class="copyright" id="copyright">(C) Copyright 2008 by HP Communications Corp.</div>
</div>
</form>
<div id="popup3_table">
<table width="60%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="19" height="37"><img src="advance/images/popup_top_left.gif" width="19" height="45" /></td>
<td background="advance/images/popup_top_center.gif" class="table_title" id=log_out>Log Out</td>
<td width="15" background="advance/images/popup_top_center.gif"><a href="#" class="iclose" onclick="MM_showHideLayers('popup','','hide','popup3_table','','hide')"></a></td>
<td width="19" height="45"><img src="advance/images/popup_top_right.gif" width="19" height="45" /></td>
</tr>
</table></td>
</tr>
<tr>
<td><table width="100%" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="19" background="advance/images/popup_main_left.gif">&nbsp;</td>
<td bgcolor="#FFFFFF"><span class="popupcontent"> 
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="27%"><center>
<img src="advance/images/i_info.jpg" width="50" height="49" />
</center>
</td>
<td width="73%" id="g2-2msg">Are you sure you want to Exit?</td>
</tr>
</table>      
</span></td>
<td width="19" background="advance/images/popup_main_right.gif">&nbsp;</td>
</tr>
</table></td>
</tr>
<tr>
<td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="19" height="48"><img src="advance/images/popup_down_left.gif" width="19" height="48" /></td>
<td background="advance/images/popup_down_center.gif">
<div class="button3"><ul>   
<li><a  href="#" title="No"><span id="g2-2No" onclick="MM_showHideLayers('popup','','hide','popup3_table','','hide')">No</span></a></li>
<li><a href="/logout.html" title="Yes"><span id="g2-2Yes">Yes</span></a></li>
</ul></div></td>
<td width="19" height="37"><img src="advance/images/popup_down_right.gif" width="19" height="48" /></td>
</tr>
</table></td>
</tr>
</table></div>
</body>
<script>
var l2tpmode = '<% getCfgZero(1, "wan_l2tp_mode"); %>';
        if (l2tpmode == "1") //DHCP for L2TP WAN IP Address assigned
        {     
	        document.wanCfg.l2tpMode[0].checked = true;
		document.wanCfg.l2tpGateway.disabled = true;
						show_div(false, "div_l2tpGateway");
		document.wanCfg.l2tpNetmask.disabled = true;
						show_div(false, "div_l2tpNetmask");
		document.wanCfg.l2tpIp.disabled = true;
						show_div(false, "div_l2tpIp");
	    }
        else //Static IP for L2TP WAN IP Address assigned
        {
            document.wanCfg.l2tpMode[1].checked = true;
		document.wanCfg.l2tpGateway.disabled = false;
						show_div(true, "div_l2tpGateway");
		document.wanCfg.l2tpNetmask.disabled = false;
						show_div(true, "div_l2tpNetmask");
		document.wanCfg.l2tpIp.disabled = false;
						show_div(true, "div_l2tpIp");
	    }
</script>
</html>
