<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside_main_frame.css?version=general_0.1.3" rel="stylesheet" type="text/css" />
<link href="images/dtree.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script>
Butterlate.setTextDomain("main");
var prev_menu_id=2;
var prev_submenu_index=1;
var polling_interval=0;
var wlan_polling_interval=0;
var cur_user = "<% getCfgGeneral(1, "AdminCurUser"); %>";
var modelname = "&nbsp;<% getCfgGeneral(1, "ProductName"); %>";
var mac_repeater = "<% getCfgGeneral(1, "ApCliMacRepeater"); %>";
var board_model = "<% getCfgZero(1, "board_model"); %>";

</script>
	<script type="text/javascript" src="images/dtree.js"></script>
    <script type="text/JavaScript">
function initTranslation()
{
	var e = document.getElementById("id_Home");
	e.innerHTML = _("Home");
	e = document.getElementById("id_Logout");
	e.innerHTML = _("Logout");
	e = document.getElementById("id_About");
	e.innerHTML = _("About");

	e = document.getElementById("id_WelcomeAdmin");
	if(cur_user == "admin") {
		e.innerHTML = _("WelcomeAdmin");
	}
	else {
		e.innerHTML = _("Welcomeguest");
	}

	e = document.getElementById("id_openall");
	e.innerHTML = _("openall");
	e = document.getElementById("id_closeall");
	e.innerHTML = _("closeall");

	e = document.getElementById("g2-2No");
	e.innerHTML = _("g2-2 No");
	e = document.getElementById("g2-2Yes");
	e.innerHTML = _("g2-2 Yes");
	e = document.getElementById("ask_logout");
	e.innerHTML = _("dash logout mes");
	e = document.getElementById("log_out");
	e.innerHTML = _("dash logout title");
	e = document.getElementById("message_header");
	e.innerHTML = _("message_header");

	e = document.getElementById("dashAboutCheck1");
	e.innerHTML = _("dash about check1");
	e = document.getElementById("dashAboutCheck2");
	e.innerHTML = _("dash about check2");
	e = document.getElementById("dashAboutClose");
	e.innerHTML = _("dash about close");
	e = document.getElementById("modelname");
	e.innerHTML = modelname;
}

<!--
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
//-->

    </script>
<style>
.on {display:on}
.off {display:none}
</style>
</head>
<body onresize="onResizeWindow()"; scroll="no" onLoad="initTranslation()">
<div id="wrapper">
  <div id="top"><div class="top_position"><div class="logo"></div>
  <div class="modelname" id="modelname">&nbsp;<% getCfgGeneral(1, "ProductName"); %></div>
  <div class="quick_link">
    <ul>
      <li style="display:none" class="about" onclick="MM_showHideLayers('popup','','show','popup4_table','','show')"><a href="#" title="About" id=id_About></a></li>
        <li class="home" style="display:none"><a href="../index_login.asp" title="Home" target="_parent" id=id_Home></a></li>
      <li onclick="MM_showHideLayers('popup3_table','','show','popup','','show')"><a href="#" title="Logout" id=id_Logout></a></li>
      <li class="line"></li>
      <li><font id=id_WelcomeAdmin></font></li>
    </ul>
  </div>
  </div></div>
  <div id="blueline"></div>
</div>
<div id="contentframe">
<div class="contentframe"><ul>
<li><div class='submenu'><ul> 

<script>
var lang = "<% get_language(); %>";

if(lang=="zhcn")
{
	//??
	document.write("<li class='i_dashboard' title='&#x72B6;&#x6001;'><a href='dashboard.asp' title='&#x72B6;&#x6001;'></a></li>");
	//???
	document.write("<li class='i_monitor' title='&#x76D1;&#x89C6;&#x5668;'></li>");

	if(cur_user == "admin")
	{
		//??
		document.write("<li class='i_config'><a href='main_config_frame.asp' title='&#x914D;&#x7F6E;'></a></li>");
		//??
		document.write("<li class='i_maintenance'><a href='main_maintenance_frame.asp' title='&#x7EF4;&#x62A4;'></a></li>");
	}
}else if(lang=="zhtw")
{
	//??
	document.write("<li class='i_dashboard' title='&#x72C0;&#x614B;'><a href='dashboard.asp' title='&#x72C0;&#x614B;'></a></li>");
	//???
	document.write("<li class='i_monitor' title='&#x76E3;&#x8996;&#x5668;'></li>");

	if(cur_user == "admin")
	{
		//??
		document.write("<li class='i_config'><a href='main_config_frame.asp' title='&#x914D;&#x7F6E;'></a></li>");
		//??
		document.write("<li class='i_maintenance'><a href='main_maintenance_frame.asp' title='&#x7DAD;&#x8B77;'></a></li>");
	}
}else
{

	document.write("<li class='i_dashboard' title='Dashboard'><a href='dashboard.asp' title='Dashboard'></a></li>");
	document.write("<li class='i_monitor' title='Monitor'></li>");
	
	if(cur_user == "admin")
	{
		document.write("<li class='i_config'><a href='main_config_frame.asp' title='Configuration'></a></li>");
		document.write("<li class='i_maintenance'><a href='main_maintenance_frame.asp' title='Maintenance'></a></li>");
	}
}
</script>

</ul>
<br class="clearfloat" /></div></li>
<li><div class="menu"> <br class="clearfloat" />
  <ul>
<li class="menuline" id=menuline_height><div><ul>
  <li class="space4"><script>document.write(_("MONITOR"))</script></li>
  <li class="space"><p><a href="javascript: d.openAll();" class="fastbutton" id=id_openall></a><a>&nbsp; | &nbsp;</a><a href="javascript: d.closeAll();" id=id_closeall></a></p></li><li>
    <div class="dtree">
<script type="text/javascript">
var opmode  = <% getCfgZero(1, "OP_Mode"); %>;

d = new dTree('d');
d.add(0,-1,'');
d.add(1,0,_('Monitor'),'');		
d.add(2,1,_('Log'),'syslog_gordon.asp', _('Monitor')+' > '+_('Log')+' > '+_('View Log'), 'InfoFrame');

/**
 * available opmode on WP688:
 * Router	0
 * AP		1
 * WISP0	6
 * STA0		8
 */
if("WP688" == board_model) {
	/***************DHCP Table***********************/
	// Router or WISP0
	if ((opmode==0) || (opmode==6)) { 
		d.add(3,1,_('DHCP Table'),'lan_dhcp_clientInfo.asp', 
			_('Monitor')+' > '+_('DHCP Table')+' > '
			+_('DHCP Table'), 'InfoFrame');
	}
	
	/***************Packet Statics***********************/
	// Any mode
	d.add(4,1,_('Packet Statistics'),'packet_statistics.asp', 
		_('Monitor')+' > '+_('Packet Statistics')+' > '
		+_('Port Statistics'), 'InfoFrame');

	/***************Association List***********************/
	// AP or Router
	if ((opmode==0) || (opmode==1)) { 
		d.add(6,1,_('WLAN 2.4G Station Status'),'assoclist.asp', 
			_('Monitor')+' > '+_('WLAN 2.4G Station Status')+' > '
			+_('Association List'), 'InfoFrame');
	}

	/***************Ethernet Client List***********************/
	//STA0
	if (opmode==8){ 
		if(mac_repeater=="1") {
			d.add(10,1,_('Ethernet Client List'),'macrepeaterclient2.asp', 
				_('Monitor')+' > '+_('Ethernet Client List')+' > '
				+_('Ethernet Client'), 'InfoFrame');
		}
		else {
			d.add(9,1,_('Ethernet Client List'),'macrepeaterclient.asp', 
				_('Monitor')+' > '+_('Ethernet Client List')+' > '
				+_('Ethernet Client'), 'InfoFrame');
		}

	}
	document.write(d);
	d.s(2);
}
/**
 * available opmode on WP838:
 * Router	0
 * AP		1
 * WISP0	6
 * WISP1	7
 * STA0		8
 * STA1		9
 */
else if("WP838" == board_model) {
	/***************DHCP Table***********************/
	//Router, WISP0 or WISP1
	if ((opmode==0) || (opmode==6) || (opmode==7)) { 
		d.add(3,1,_('DHCP Table'),'lan_dhcp_clientInfo.asp', 
			_('Monitor')+' > '+_('DHCP Table')+' > '
			+_('DHCP Table'), 'InfoFrame');
	}

	/***************Packet Statics***********************/
	// Any mode
	d.add(4,1,_('Packet Statistics'),'packet_statistics.asp', 
		_('Monitor')+' > '+_('Packet Statistics')+' > '
		+_('Port Statistics'), 'InfoFrame');

	/***************Association List***********************/
	// AP or Router
	if ((opmode==0) || (opmode==1)) { 
		//CPE_5G
		
		//d.add(6,1,_('WLAN 2.4G Station Status'),'assoclist.asp', 
		//	_('Monitor')+' > '+_('WLAN 2.4G Station Status')+' > '
		//	+_('Association List'), 'InfoFrame');

		//d.add(8,1,_('WLAN 5G Station Status'),'assoclist_5g.asp', 
		//	_('Monitor')+' > '+_('WLAN 5G Station Status')+' > '
		//	+_('Association List'), 'InfoFrame');

		d.add(6,1,_('WLAN Station Status'),'assoclist.asp', 
			_('Monitor')+' > '+_('WLAN Station Status')+' > '
			+_('Association List'), 'InfoFrame');
	}
	// STA0 or WISP0
	else if((opmode==6) || (opmode==8)) {
		//CPE_5G
		//d.add(8,1,_('WLAN 5G Station Status'),'assoclist_5g.asp', 
		//	_('Monitor')+' > '+_('WLAN 5G Station Status')+' > '
		//	+_('Association List'), 'InfoFrame');
	}
	// STA1 or WISP1
	else if((opmode==7) || (opmode==9)) {
		//CPE_5G
		//d.add(6,1,_('WLAN 2.4G Station Status'),'assoclist.asp', 
		//	_('Monitor')+' > '+_('WLAN 2.4G Station Status')+' > '
		//	+_('Association List'), 'InfoFrame');

		d.add(6,1,_('WLANStation Status'),'assoclist.asp', 
			_('Monitor')+' > '+_('WLAN Station Status')+' > '
			+_('Association List'), 'InfoFrame');
	}

	/***************Ethernet Client List***********************/
	// STA0 or STA1
	if ((opmode==8) || (opmode==9)){ 
		if(mac_repeater=="1") {
			d.add(10,1,_('Ethernet Client List'),'macrepeaterclient2.asp', 
				_('Monitor')+' > '+_('Ethernet Client List')+' > '
				+_('Ethernet Client'), 'InfoFrame');
		}
		else {
			d.add(9,1,_('Ethernet Client List'),'macrepeaterclient.asp', 
				_('Monitor')+' > '+_('Ethernet Client List')+' > '
				+_('Ethernet Client'), 'InfoFrame');
		}
	}

	document.write(d);
	d.s(2);

}
//TODO
else if("WP868" == board_model) {
}

</script>
</div></li></ul> 
  </div></li> 
	  </ul> <br class="clearfloat" /></div>
</li></ul> <br class="clearfloat" /></div>
<div class="contenleft">
  <div class="path">
    <ul>
      <li class="i_path"><a class="w_path" id="pathTitle"></a></li>
    </ul>
    <br class="clearfloat" />
  </div>
<span id=message_show class='off'>  
  <div class="barcontent" id=message_position>
    <ul>
      <li class="i_message">
      <table width="100%" border="0" cellspacing="0" cellpadding="0"><tr><td nowrap>&nbsp;&nbsp;&nbsp;
      <a class="message_title" style="color:#30466d;" id="message_header">Message</a>:
      </td></tr></table>
      </li>
			<li class="message_word" id=message_len>
			<table width="100%" border="0" cellspacing="0" cellpadding="0"><tr><td nowrap>
			<a style="color:#3d8900;" id=message_str>Ready</a>
			</td></tr></table>
			</li>
    </ul>
    <br class="clearfloat" />
  </div>
</span>  
  <div class="contentmain">
    <div id="tabs">
      <ul>
<!-- Log -->
<span id = "menu_2_1_hightline" class = "off" >
        <li class="hightline"><a title="View Log"><span><script>document.write(_("View Log"))</script></span></a></li>
</span> 
<span id = "menu_2_1" class = "off" >
        <li><a target="InfoFrame" href="syslog_gordon.asp" title="View Log" onclick="onChangeHeadMenu(2, 1, _('Monitor')+' > '+_('Log')+' > '+_('View Log'))"><span><script>document.write(_("View Log"))</script></span></a></li>
</span>
<span id = "menu_2_2_hightline" class = "off" >
        <li class="hightline"><a title="Log Settings"><span><script>document.write(_("Log Settings"))</script></span></a></li>
</span>    
<span id = "menu_2_2" class = "off" >
        <li><a target="InfoFrame" href="LogSettings.asp" title="Log Settings" onclick="onChangeHeadMenu(2, 2, _('Monitor')+' > '+_('Log')+' > '+_('Log Settings'))"><span><script>document.write(_("Log Settings"))</script></span></a></li>
</span>


<span id = "menu_3_1_hightline" class = "off" >
        <li class="hightline"><a title="DHCP Table"><span><script>document.write(_("DHCP Table"))</script></span></a></li>
</span>  
<span id = "menu_3_1" class = "off" >
        <li><a target="InfoFrame" href="lan_dhcp_clientInfo.asp" title="DHCP Table" onclick="onChangeHeadMenu(3, 1, _('Monitor')+' > '+_('DHCP Table')+' > '+_('DHCP Table'))"><span><script>document.write(_("DHCP Table"))</script></span></a></li>
</span>  


<!-- Packet Statistics -->    
<span id = "menu_4_1_hightline" class = "off" >
        <li class="hightline"><a title="Port Statistics"><span><script>document.write(_("Port Statistics"))</script></span></a></li>
</span>    
<span id = "menu_4_1" class = "off" >
        <li><a target="InfoFrame" href="packet_statistics.asp" title="Port Statistics" onclick="onChangeHeadMenu(4, 1, _('Monitor')+' > '+_('Packet Statistics')+' > '+_('Port Statistics'))"><span><script>document.write(_("Port Statistics"))</script></span></a></li>
</span>
<span id = "menu_4_2_hightline" class = "off" >
        <li class="hightline"><a title="Wireless Statistics"><span><script>document.write(_("Wireless Statistics"))</script></span></a></li>
</span>    
<span id = "menu_4_2" class = "off" >
        <li><a target="InfoFrame" href="packet_statistics_wl.asp" title="Wireless Statistics" onclick="onChangeHeadMenu(4, 2, _('Monitor')+' > '+_('Packet Statistics')+' > '+_('Wireless Statistics'))"><span><script>document.write(_("Wireless Statistics"))</script></span></a></li>
</span> 


<!-- WLAN Station Status -->    
<span id = "menu_5_1_hightline" class = "off" >
        <li class="hightline"><a title="Association List"><span><script>document.write(_("Association List"))</script></span></a></li>
</span>    
<span id = "menu_5_1" class = "off" >
        <li><a target="InfoFrame" href="assoclist.asp" title="Association List" onclick="onChangeHeadMenu(5, 1, _('Monitor')+' > '+_('WLAN 2.4G-1 Station Status')+' > '+_('Association List'))"><span><script>document.write(_("Association List"))</script></span></a></li>
</span>


<span id = "menu_6_1_hightline" class = "off" >
        <li class="hightline"><a title="Association List"><span><script>document.write(_("Association List"))</script></span></a></li>
</span>    
<span id = "menu_6_1" class = "off" >
        <li><a target="InfoFrame" href="assoclist.asp" title="Association List" onclick="onChangeHeadMenu(6, 1, _('Monitor')+' > '+_('WLAN 2.4G Station Status')+' > '+_('Association List'))"><span><script>document.write(_("Association List"))</script></span></a></li>
</span> 


<span id = "menu_7_1_hightline" class = "off" >
        <li class="hightline"><a title="Association List"><span><script>document.write(_("Association List"))</script></span></a></li>
</span>    
<span id = "menu_7_1" class = "off" >
        <li><a target="InfoFrame" href="assoclist.asp" title="Association List" onclick="onChangeHeadMenu(7, 1, _('Monitor')+' > '+_('WLAN 2.4G-2 Station Status')+' > '+_('Association List'))"><span><script>document.write(_("Association List"))</script></span></a></li>
</span>


<span id = "menu_8_1_hightline" class = "off" >
        <li class="hightline"><a title="Association List"><span><script>document.write(_("Association List"))</script></span></a></li>
</span>    

<!-- CPE_5G -->  
<span id = "menu_8_1" class = "off" style="display:none">
        <li><a target="InfoFrame" href="assoclist_5g.asp" title="Association List" onclick="onChangeHeadMenu(8, 1, _('Monitor')+' > '+_('WLAN 5G Station Status')+' > '+_('Association List'))"><span><script>document.write(_("Association List"))</script></span></a></li>
</span>


<span id = "menu_9_1_hightline" class = "off" >
        <li class="hightline"><a title="Ethernet Client"><span><script>document.write(_("Ethernet Client"))</script></span></a></li>
</span>
<span id = "menu_9_1" class = "off" >
        <li><a target="InfoFrame" href="macrepeaterclient.asp" title="Ethernet Client" onclick="onChangeHeadMenu(9, 1, _('Monitor')+' > '+_('Ethernet Client List')+' > '+_('Ethernet Client'))"><span><script>document.write(_("Ethernet Client"))</script></span></a></li>
</span>


<span id = "menu_10_1_hightline" class = "off" >
        <li class="hightline"><a title="Ethernet Client"><span><script>document.write(_("Ethernet Client"))</script></span></a></li>
</span>
<span id = "menu_10_1" class = "off" >
        <li><a target="InfoFrame" href="macrepeaterclient2.asp" title="Ethernet Client" onclick="onChangeHeadMenu(10, 1, _('Monitor')+' > '+_('Ethernet Client List')+' > '+_('Ethernet Client'))"><span><script>document.write(_("Ethernet Client"))</script></span></a></li>
</span>
      </ul>
      <br class="clearfloat" />
    </div>
    <div id="table">
      <ul>
        <li class="table_top"></li>
        <li class="table_content"><br />
            <div class="data">
              <ul>
<table width="100%" border="0" cellspacing="0" cellpadding="0"><tr><td>
<script>
var current_url=location.href;
var current_url_len=current_url.length;
var index=current_url.charAt(current_url_len-1);

/**
 * available opmode on WP688:
 * Router	0
 * AP		1
 * WISP0	6
 * STA0		8
 */
if("WP688" == board_model) {
	if (((opmode==0)||(opmode==6)) && index==1) {
		document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/lan_dhcp_clientInfo.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>"); 
	}
	else if (index==2) {
		document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/packet_statistics.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");     
	}
	else if (((opmode==0)||(opmode==1)) && index==3) {
		document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/assoclist.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");              
	}
	else {
		document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/syslog_gordon.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");  
	}
}
/**
 * available opmode on WP838:
 * Router	0
 * AP		1
 * WISP0	6
 * WISP1	7
 * STA0		8
 * STA1		9
 */
else if("WP838" == board_model) {
	if (((opmode==0)||(opmode==6)||(opmode==7)) && index==1) {
		document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/lan_dhcp_clientInfo.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>"); 
	}
	else if (index==2) {
		document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/packet_statistics.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");     
	}
	else if (((opmode==0)||(opmode==1)||(opmode==7)||(opmode==9)) && index==3) {
		document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/assoclist.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");              
	}
	//CPE_5G
	/*
	else if (((opmode==0)||(opmode==1)||(opmode==6)||(opmode==8)) && index==4) {
		document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/assoclist_5g.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");              
	}
	*/
	else {
		document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/syslog_gordon.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");  
	}

}
//TODO
else if("WP868" == board_model) {
}


</script>
</iframe>
</td></tr></table>
        </ul>
        </div>
        </li>
      </ul>
      <br class="clearfloat" />
    </div>
<br /><br /><br /><br />
  </div>
</div>
</div>
<div id="popup3_table">
  <table width="60%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td width="19" height="37"><img src="images/popup_top_left.gif" width="19" height="45" /></td>
        <td background="images/popup_top_center.gif" class="table_title" id=log_out>Log Out</td>
        <td width="15" background="images/popup_top_center.gif"><a href="#" class="iclose" onclick="MM_showHideLayers('popup3_table','','hide','popup','','hide')"></a></td>
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
              <td width="27%"><center>
                <img src="images/i_info.jpg" width="50" height="49" />
              </center>
              </td>
              <td width="73%" id=ask_logout>Are you sure you want to log out?</td>
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
        <td background="images/popup_down_center.gif"><div class="button3">
          <ul>   
    <li><a href="#" title="No"><span id="g2-2No" onclick="MM_showHideLayers('popup3_table','','hide','popup','','hide')">No</span></a></li>
	<li><a href="/logout.html" title="Yes"><span id="g2-2Yes">Yes</span></a></li>
	</ul>
        </div></td>
        <td width="19" height="37"><img src="images/popup_down_right.gif" width="19" height="48" /></td>
      </tr>
    </table></td>
  </tr>
</table></div>
<div id="popup4_table">
  <table width="60%" border="0" cellspacing="0" cellpadding="0">
    <tr>
      <td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
        <tr>
          <td width="19" height="37"><img src="images/popup_top_left.gif" width="19" height="45" /></td>
          <td background="images/popup_top_center.gif" class="table_title">About</td>
          <td width="15" background="images/popup_top_center.gif"><a href="#" class="iclose" onclick="MM_showHideLayers('popup','','hide','popup4_table','','hide')"></a></td>
          <td width="19" height="45"><img src="images/popup_top_right.gif" width="19" height="45" /></td>
        </tr>
      </table></td>
    </tr>
    <tr>
      <td><table width="100%" border="0" cellpadding="0" cellspacing="0">
        <tr>
          <td width="19" background="images/popup_main_left.gif">&nbsp;</td>
          <td bgcolor="#FFFFFF"><span class="popupcontent">
            <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0" style="display:none">
              <tr>
                <td width="27%"><center>
                  <img src="images/i_info.jpg" width="50" height="49" />
                </center></td>
                <td width="73%"><span class="blod"><% getCfgGeneral(1, "ProductName"); %></span> <br />
                  (C) Copyright@2014 Howen Technologies Co.,Ltd.<br />
                  <br />
                  <font id="dashAboutCheck1"></font> <a href="http://www.howentech.com" target="_blank">www.howentech.com</a> <font id="dashAboutCheck2"></font></td>
              </tr>
              <tr>
                <td height="20" colspan="2"><center>
                </center></td>
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
          <td background="images/popup_down_center.gif"><div class="button3">
            <ul>
              <li><a href="#" title="Close"><span onclick="MM_showHideLayers('popup','','hide','popup4_table','','hide')"><font id="dashAboutClose"></font></span></a></li>
            </ul>
          </div></td>
          <td width="19" height="37"><img src="images/popup_down_right.gif" width="19" height="48" /></td>
        </tr>
      </table></td>
    </tr>
  </table>
</div>
<div id="popup"></div>
</body>
<script>
var current_url=location.href;
var current_url_len=current_url.length;
var index=current_url.charAt(current_url_len-1);
var current_menu_id=2;
var current_submenu_index=1;

/**
 * available opmode on WP688:
 * Router	0
 * AP		1
 * WISP0	6
 * STA0		8
 * available index on WP688
 * index 1	DHCP Table
 * index 2	Packet Statistics
 * index 3	Station List 2.4G
 */
if("WP688" == board_model) {
	//index 1 for DHCP Table, only for Router, WISP0
	if (((opmode==0)||(opmode==6)) && index==1) {
		current_menu_id=3;
		current_submenu_index=1;
		onChangeHeadMenu(current_menu_id, current_submenu_index, _('Monitor')+' > '+_('DHCP Table')+' > '+_('DHCP Table'));
		d.s(3);
	}
	//index 2 for Packet Statistics for any mode
	else if (index==2) {
		current_menu_id=4;
		current_submenu_index=1;
		onChangeHeadMenu(current_menu_id, current_submenu_index, _('Monitor')+' > '+_('Packet Statistics')+' > '+_('Port Statistics'));
		// Router and WISP0
		if ((opmode==0) || (opmode==6)) {
			d.s(4);
		}
		else {
			d.s(3);
		}
	}
	//index 3 for STA List 2.4G, only for AP and Router Mode
	else if (((opmode==0)||(opmode==1)) && index==3) {
		current_menu_id=5;
		current_submenu_index=1;
		
		//CPE_5G
		//onChangeHeadMenu(current_menu_id, current_submenu_index, _('Monitor')+' > '+_('WLAN 2.4G Station Status')+' > '+_('Association List'));
		onChangeHeadMenu(current_menu_id, current_submenu_index, _('Monitor')+' > '+_('WLAN 2.4G Station Status')+' > '+_('Association List'));

		// Router 
		if (opmode==0)  {
			d.s(5);
		}
		// AP
		else if(opmode==1){
			d.s(4);
		}
	}
	//without index
	else {
		onChangeHeadMenu(current_menu_id, current_submenu_index, _('Monitor')+' > '+_('Log')+' > '+_('View Log'));
		d.s(2);
	}
}
/**
 * available opmode on WP838:
 * Router	0
 * AP		1
 * WISP0	6
 * WISP1	7
 * STA0		8
 * STA1		9
 * available index on WP838
 * index 1	DHCP Table
 * index 2	Packet Statistics
 * index 3	Station List 2.4G
 * index 4	Station List 5G
 */
else if("WP838" == board_model) {
	//index 1 for DHCP Table, only for Router, WISP0, WISP1
	if (((opmode==0)||(opmode==6)||(opmode==7)) && index==1) {
		current_menu_id=3;
		current_submenu_index=1;
		onChangeHeadMenu(current_menu_id, current_submenu_index, _('Monitor')+' > '+_('DHCP Table')+' > '+_('DHCP Table'));
		d.s(3);
	}
	//index 2 for Packet Statistics for any mode
	else if (index==2) {
		current_menu_id=4;
		current_submenu_index=1;
		onChangeHeadMenu(current_menu_id, current_submenu_index, _('Monitor')+' > '+_('Packet Statistics')+' > '+_('Port Statistics'));
		// Router, WISP0, WISP1
		if ((opmode==0) || (opmode==6) || (opmode==7)) {
			d.s(4);
		}
		//AP, STA0, STA1
		else {
			d.s(3);
		}
	}
	//index 3 for STA List 2.4G for Router, AP, WISP1, STA1
	else if (((opmode==0)||(opmode==1)||(opmode==7)||(opmode==9)) && index==3) {
		current_menu_id=5;
		current_submenu_index=1;

		//CPE_5G
		//onChangeHeadMenu(current_menu_id, current_submenu_index, _('Monitor')+' > '+_('WLAN 2.4G Station Status')+' > '+_('Association List'));
		onChangeHeadMenu(current_menu_id, current_submenu_index, _('Monitor')+' > '+_('WLAN Station Status')+' > '+_('Association List'));

		// Router, WISP1
		if ((opmode==0) || (opmode==7)) {
			d.s(5);
		}
		// AP, STA1
		else {
			d.s(4);
		}
	}
	//index 4 for STA List 5G for Router, AP, WISP0, STA0
	else if (((opmode==0)||(opmode==1)||(opmode==6)||(opmode==8)) && index==4) {
		current_menu_id=8;
		current_submenu_index=1;
		onChangeHeadMenu(current_menu_id, current_submenu_index, _('Monitor')+' > '+_('WLAN 5G Station Status')+' > '+_('Association List'));
		// Router
		if (opmode==0) {
			d.s(6);
		}
		// AP
		else if (opmode==1) {
			d.s(5);
		}
		// WISP0
		else if(opmode==6) {
			d.s(5);
		}
		// STA0
		else {
			d.s(4);
		}
	}
	//without index
	else {
		onChangeHeadMenu(current_menu_id, current_submenu_index, _('Monitor')+' > '+_('Log')+' > '+_('View Log'));
		d.s(2);
	}

}
//TODO
else if("WP868" == board_model) {
}

function onResizeWindow()
{
	adjustMyFrameHeight();
}
</script>	
</html>
