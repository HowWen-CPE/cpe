<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/expert.css?version=general_0.1.3" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="/DST.js"></script>
<script>
Butterlate.setTextDomain("main");
</script>
<script type="text/JavaScript">
<!--
function ssid(ssid)
{
	var i = 0;
	var s;
	var s1;
	var tmp_ssid = new Array();
	for(i = 0; i < ssid.length; i++ )
	{
		s = ssid.charAt(i);
		s1 = ssid.charCodeAt(i);
		if(s1 == 32)
			tmp_ssid[i] = "&nbsp;";
		else
			tmp_ssid[i] = s;
	}
				
	 document.write(tmp_ssid.join(""));
}

function MM_swapImgRestore() { //v3.0
  var i,x,a=document.MM_sr; for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) x.src=x.oSrc;
}

function MM_preloadImages() { //v3.0
  var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();
    var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)
    if (a[i].indexOf("#")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}
}

function MM_findObj(n, d) { //v4.01
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && d.getElementById) x=d.getElementById(n); return x;
}

function MM_swapImage() { //v3.0
  var i,j=0,x,a=MM_swapImage.arguments; document.MM_sr=new Array; for(i=0;i<(a.length-2);i+=3)
   if ((x=MM_findObj(a[i]))!=null){document.MM_sr[j++]=x; if(!x.oSrc) x.oSrc=x.src; x.src=a[i+2];}
}

function MM_showHideLayers() { //v6.0
  var i,p,v,obj,args=MM_showHideLayers.arguments;
  for (i=0; i<(args.length-2); i+=3) if ((obj=MM_findObj(args[i]))!=null) { v=args[i+2];
    if (obj.style) { obj=obj.style; v=(v=='show')?'visible':(v=='hide')?'hidden':v; }
    obj.visibility=v; }
}
//-->

var nv_OP_Mode = <% getCfgZero(1, "OP_Mode"); %>;

var addrmode = "<% getCfgGeneral(1, "ApCliAddrMode"); %>";

//var wan_ipv6_proto = '<% getCfgZero(1, "wan_ipv6_proto"); %>';
var board_type = "<% getCfgZero(1, "board_model"); %>";

function initTranslation()
{
	var e;

	e = document.getElementById("dash_infoTitle");
	e.innerHTML = _("dash_info title");
	e = document.getElementById("dash_infoItem");
	e.innerHTML = _("dash_info item");
	e = document.getElementById("dash_infoData");
	e.innerHTML = _("dash_info data");
	e = document.getElementById("dash_infoProductName");
	e.innerHTML = _("dash_info productname");
	e = document.getElementById("dash_infoHostName");
	e.innerHTML = _("dash_info hostname");
	e = document.getElementById("dash_infoFwVer");
	e.innerHTML = _("dash_info fwver");
	e = document.getElementById("dash_infoSysOpmode");
	e.innerHTML = _("dash_info sys opmode");

	if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6)||(nv_OP_Mode == 7)){
		e = document.getElementById("dash_infoWanInfo");
		e.innerHTML = _("dash_info wan info");
 		
		e = document.getElementById("dash_infoWanMac");
		e.innerHTML = _("dash_info wan mac");
		e = document.getElementById("dash_infoWanIp");
		e.innerHTML = _("dash_info wan ip");
		e = document.getElementById("dash_infoWanSubmask");
		e.innerHTML = _("dash_info wan mask");
		e = document.getElementById("dash_infoWanContype");
		e.innerHTML = _("dash_info wan contype");
		
	}

	e = document.getElementById("dash_infoLanInfo");
	e.innerHTML = _("dash_info lan info");
	e = document.getElementById("dash_infoLanMac");
	e.innerHTML = _("dash_info lan mac");
	e = document.getElementById("dash_infoLanIp");
	e.innerHTML = _("dash_info lan ip");
	e = document.getElementById("dash_infoLanSubmask");
	e.innerHTML = _("dash_info lan mask");
	
	e = document.getElementById("dash_infoSystatTitle");
	e.innerHTML = _("dash_info systat title");
	e = document.getElementById("dash_infoSystatItem");
	e.innerHTML = _("dash_info systat item");
	e = document.getElementById("dash_infoSystatData");
	e.innerHTML = _("dash_info systat data");
	e = document.getElementById("dash_infoSystatSysuptime");
	e.innerHTML = _("dash_info systat sysuptime");
	e = document.getElementById("dash_infoSystatCurrtime");
	e.innerHTML = _("dash_info systat currtime");
	e = document.getElementById("dash_infoSystatSysres");
	e.innerHTML = _("dash_info systat sysres");
	e = document.getElementById("dash_infoSystatCpu");
	e.innerHTML = _("dash_info systat cpu");
	e = document.getElementById("dash_infoSystatMem");
	e.innerHTML = _("dash_info systat mem");
	//e = document.getElementById("dash_infoSystatSetting");
	//e.innerHTML = _("dash_info systat setting");
	
	if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6) ||(nv_OP_Mode == 7)){
		e = document.getElementById("dash_infoSystatSettingFw");
		e.innerHTML = _("dash_info systat setting fw");
		//e = document.getElementById("dash_infoSystatBm");
		//e.innerHTML = _("dash_info systat bm");
	}

	//e = document.getElementById("dash_infoSystatConfmode");
	//e.innerHTML = _("dash_info systat confmode");
	//e = document.getElementById("dash_infoSystatConfmodeExp");
	//e.innerHTML = _("dash_info systat confmode exp");

/*	//Deleted by Andy Yu in 2013/07/11: IPV6
	//Steve add for IPv6 Status
	if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6)||(nv_OP_Mode == 7)){
	
		e = document.getElementById("dash_infoIPv6Title");
		e.innerHTML = _("dash_info ipv6 title");
		e = document.getElementById("dash_infoIPv6Item");
		e.innerHTML = _("dash_info systat item");
		e = document.getElementById("dash_infoIPv6Data");
		e.innerHTML = _("dash_info systat data");
		e = document.getElementById("dash_infoIPv6ConnType");
		e.innerHTML = _("dash_info ipv6 conn type");
		if((wan_ipv6_proto == '0') || (wan_ipv6_proto == '1')){ 
			e = document.getElementById("dash_infoIPv6WanAddress");
			e.innerHTML = _("dash_info ipv6 wan address");	
			e = document.getElementById("dash_infoIPv6WanGateway");
			e.innerHTML = _("dash_info ipv6 wan gateway");
			e = document.getElementById("dash_infoIPv6WanFirstDns");
			e.innerHTML = _("dash_info ipv6 wan first dns");
			e = document.getElementById("dash_infoIPv6WanSecondDns");
			e.innerHTML = _("dash_info ipv6 wan second dns");
			e = document.getElementById("dash_infoIPv6LanAddress");
			e.innerHTML = _("dash_info ipv6 lan address");
		}
		e = document.getElementById("dash_infoIPv6LanLinkLocal");
		e.innerHTML = _("dash_info ipv6 lan link local");	
	}
*/
	e = document.getElementById("dash_infoSumTitle");
	e.innerHTML = _("dash_info sum title");
	
	if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6)||(nv_OP_Mode == 7)){
		//e = document.getElementById("dash_infoSumBw");
		//e.innerHTML = _("dash_info sum bw");
		//e = document.getElementById("dash_infoSumBwDet");
		//e.innerHTML = _("dash_info sum bw det");
		e = document.getElementById("dash_infoSumDhcp");
		e.innerHTML = _("dash_info sum dhcp");
		e = document.getElementById("dash_infoSumDhcpDet");
		e.innerHTML = _("dash_info sum dhcp det");
	}
	
	e = document.getElementById("dash_infoSumPktstat");
	e.innerHTML = _("dash_info sum pktstat");
	e = document.getElementById("dash_infoSumPktstatDet");
	e.innerHTML = _("dash_info sum pktstat det");
	
	if ((nv_OP_Mode == 0)||(nv_OP_Mode == 1)||(nv_OP_Mode == 7)||(nv_OP_Mode == 9)){
		e = document.getElementById("dash_infoSumWlansta");
		e.innerHTML = _("dash_info sum wlan");
	    e = document.getElementById("dash_infoSumWlanstaDet");
		e.innerHTML = _("dash_info sum wlan det");
	}

	if ((board_type == "WP838")&&((nv_OP_Mode == 0)||(nv_OP_Mode == 1)||(nv_OP_Mode == 6)||(nv_OP_Mode == 8))){
		e = document.getElementById("dash_infoSumWlan1sta");
		e.innerHTML = _("dash_info sum wlan1");
	    e = document.getElementById("dash_infoSumWlan1staDet");
		e.innerHTML = _("dash_info sum wlan det");
	}
}

function wlan5g_ap_channel_mode(){

	var ieee802_1x = "<% getCfgZero(1, 'IEEE8021X'); %>"
	var Auth = "<% getCfgZero(1, 'Auth5GMode'); %>"
	var Encry = "<% getCfgZero(1, 'Encryp5GType'); %>"
	
	//Main_SSID
	document.write("<tr >");
    document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanStatusNamessid'>(SSID)</font>:</span></td>");
    e = document.getElementById("dash1_infoWlanStatusNamessid");
    e.innerHTML = _("dash_info wlan status namessid");
    document.write("<td ><span class='table_font2'><% getWLAN5GMainSSID(); %></span></td>");
    document.write("</tr>");

	//Band_Width
	document.write("<tr >");
    document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanChanbw'></font>:</span></td>");
    e = document.getElementById("dash1_infoWlanChanbw");
	e.innerHTML = _("dash_info wlan chanbw");
    document.write("<td ><span class='table_font2'><% getChannelBandWidth5G(); %></span></td>");
    document.write("</tr>");

    //Select_Way
    document.write("<tr >");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanChan'></font>:</span></td>");
	e = document.getElementById("dash1_infoWlanChan");
    e.innerHTML = _("dash_info wlan chan");
	document.write("<td >");
	if (<% getWLAN5GChannelSelectWay(); %> == 0 ){
		document.write("<span class='table_font2'><font id='dash1_infoWlanChanAuto'></font></span>");
		e = document.getElementById("dash1_infoWlanChanAuto");
		e.innerHTML = _("dash_info wlan chan auto");
	}else
	{
		document.write("<span class='table_font2'><font id='dash1_infoWlanChanfixed'></font></span>");
		e = document.getElementById("dash1_infoWlanChanfixed");
		e.innerHTML = _("dash_info wlan chan fixed");
	}
	document.write("</td>");
	document.write("</tr>");

	//OPChan
	document.write("<tr >");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanOpchan'></font>:</span></td>");
	document.write("<td ><span class='table_font2'><% getOpChannleASP5G(); %></span></td>");
	e = document.getElementById("dash1_infoWlanOpchan");
    e.innerHTML = _("dash_info wlan opchan");
	document.write("</tr>");

	//ExtChan
	document.write("<tr >");
    document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanExtchan'></font>:</span></td>");
    e = document.getElementById("dash1_infoWlanExtchan");
	e.innerHTML = _("dash_info wlan extchan");
    document.write("<td ><span class='table_font2'><% getExtChannel5G(); %></span></td>");
    document.write("</tr>");

    //SecMode
    document.write("<tr>");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanSec'></font>:</span></td>");
	e = document.getElementById("dash1_infoWlanSec");
	e.innerHTML = _("dash_info wlan sec");
	document.write("<td>");
	if (ieee802_1x.charAt(0) == 1 ){
		document.write("<span class='table_font2'>802.1x</span>");
	}else if ((Auth.split(";",1) == "OPEN") && (Encry.split(";",1) == "NONE")){
		document.write("<span class='table_font2'><font id='dash1_infoWlanSecNosec2'></font></span>");
		e = document.getElementById("dash1_infoWlanSecNosec2");
		e.innerHTML = _("dash_info wlan sec nosec");
	}else if (Auth.split(";",1) == "SHARED"){
		document.write("<span class='table_font2'><font id='dash1_infoWlanSecWEP1'></font></span>");
		e = document.getElementById("dash1_infoWlanSecWEP1");
		e.innerHTML = _("dash_info wlan sec wep");
	}else if (Auth.split(";",1) == "WPAPSK"){
		document.write("<span class='table_font2'><font id='dash1_infoWlanSecWPAPSK1'></font></span>");
		e = document.getElementById("dash1_infoWlanSecWPAPSK1");
		e.innerHTML = _("dash_info wlan sec wpapsk");
	}else if (Auth.split(";",1) == "WPA2PSK"){
		document.write("<span class='table_font2'><font id='dash1_infoWlanSecWPA2PSK1'></font></span>");
		e = document.getElementById("dash1_infoWlanSecWPA2PSK1");
		e.innerHTML = _("dash_info wlan sec wpa2psk");
	}else if (Auth.split(";",1) == "WPAPSKWPA2PSK"){
		document.write("<span class='table_font2'><font id='dash1_infoWlanSecWPAPSKWPA2PSK'></font></span>");
		e = document.getElementById("dash1_infoWlanSecWPAPSKWPA2PSK");
		e.innerHTML = _("dash_info wlan sec wpapskwpa2psk");
	}else if (Auth.split(";",1) == "WPAWPA2"){
		document.write("<span class='table_font2'><font id='dash1_infoWlanSecWPAWPA2'></font></span>");
		e = document.getElementById("dash1_infoWlanSecWPAWPA2");
		e.innerHTML = _("dash_info wlan sec wpawpa2");
	}else{
		document.write("<span class='table_font2'>"+Auth.split(";",1)+"</span>");
	}
	document.write("</td>");
	document.write("</tr>");

	//wirelee_mode
	document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlan80211mode'></font>:</span></td><td>");
	e = document.getElementById("dash1_infoWlan80211mode");
	e.innerHTML = _("dash_info wlan 80211mode");
	var wireless5g_mode = <% getWLAN5GWirelessMode(); %>;
	if (wireless5g_mode == 8){
	    document.write("<span class='table_font2'>802.11an</span>");
	}else if (wireless5g_mode == 14){
		document.write("<span class='table_font2'>802.11ac</span>");
	}else{
	   document.write("<span class='table_font2'>802.11an</span>");
	}
	document.write("</td></tr>");
}

function wlan5g_cli_channel_mode()
{
	//Associated_SSID
	document.write("<tr >");
    document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanStatusNameEssid'></font>:</span></td>");
    e = document.getElementById("dash1_infoWlanStatusNameEssid");
    e.innerHTML = _("dash_info wlan associated namessid");
    
    var associate_status="<% getWISP1apcli0ConnectStatus(); %>";
    if (associate_status=="None")
		document.write("<td ><span class='table_font2'> </span></td>");
	else {
		document.write("<td ><span class='table_font2'>");
		document.write("<% getWISP1apcli0ConnectESSID(); %><% getWISP1apcli0ConnectESSIDMAC(); %></span></td>");
	}
    document.write("</tr>");

    //Connect_Status
    document.write("<tr >");
    document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanWISPConnectionStatus'></font>:</span></td>");
    e = document.getElementById("dash1_infoWlanWISPConnectionStatus");
    e.innerHTML = _("Connect Status");
    
    if (associate_status=="None")
    	document.write("<td ><span class='table_font2'>"+_('Disassociated')+"</span></td>");
       			
    if (associate_status=="Connected")
       	document.write("<td ><span class='table_font2'>"+_('Associated')+"</span></td>");
       			
    document.write("</tr>");

    //RSSI
    if(associate_status=="Connected") {
		document.write("<tr>");
		document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanWISPRSSI'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getwifi5GRSSI(); %></span></td>");
		document.write("</tr>");
		e = document.getElementById("dash1_infoWlanWISPRSSI");
		e.innerHTML=_("dash_info rssi");
	}

	//Sec_mode
    var Secu = <% getWISP1apcli0AuthMode(); %>;
    document.write("<tr>");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanSec'></font>:</span></td>");
	e = document.getElementById("dash1_infoWlanSec");
	e.innerHTML = _("dash_info wlan sec");
	document.write("<td>");
    if (Secu == 0){
        document.write("<span class='table_font2'><font id='dash_infoWlanSecNosec1'></font></span>");
        e = document.getElementById("dash_infoWlanSecNosec1");
        e.innerHTML = _("dash_info wlan sec nosec");			
    }else if (Secu == 1){
        document.write("<span class='table_font2'><font id='dash_infoWlanSecWPAPSK'></font></span>");
        e = document.getElementById("dash_infoWlanSecWPAPSK");
        e.innerHTML = _("dash_info wlan sec wpapsk");
    }else if (Secu == 2){
        document.write("<span class='table_font2'><font id='dash_infoWlanSecWPA2PSK'></font></span>");
        e = document.getElementById("dash_infoWlanSecWPA2PSK");
        e.innerHTML = _("dash_info wlan sec wpa2psk");
    }else if (Secu == 3){
        document.write("<span class='table_font2'><font id='dash_infoWlanSecWPAPSKWPA2PSK'></font></span>");
        e = document.getElementById("dash_infoWlanSecWPAPSKWPA2PSK");
        e.innerHTML = _("dash_info wlan sec wpapskwpa2psk");
    }else if (Secu == 4){
        document.write("<span class='table_font2'><font id='dash_infoWlanSecWPA'></font></span>");
        e = document.getElementById("dash_infoWlanSecWPA");
        e.innerHTML = _("dash_info wlan sec wpa");
    }else if (Secu == 5){
        document.write("<span class='table_font2'><font id='dash_infoWlanSecWPA2'></font></span>");
        e = document.getElementById("dash_infoWlanSecWPA2");
        e.innerHTML = _("dash_info wlan sec wpa2");
    }else if (Secu == 6){
        document.write("<span class='table_font2'><font id='dash_infoWlanSecWPAWPA2'></font></span>");
        e = document.getElementById("dash_infoWlanSecWPAWPA2");
        e.innerHTML = _("dash_info wlan sec wpawpa2");
    }else if (Secu == 7){
        document.write("<span class='table_font2'><font id='dash_infoWlanSecWEP'></font></span>");
        e = document.getElementById("dash_infoWlanSecWEP");
        e.innerHTML = _("dash_info wlan sec wep");
    }
	document.write("</td></tr>");
}

function wlan5g_information() {

	var e;
	document.write("<tr>");
	document.write("<td valign='top'><span class='table_font'><font id='dash1_infoWlanInfo'></font>:</span></td>");
	e = document.getElementById("dash1_infoWlanInfo");
	e.innerHTML = _("dash_info wlan1 info");
	document.write("<td><span class='table_font2'></span></td>");
	document.write("</tr>");
	document.write("<tr>");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanOpmode'></font>: </span></td>");
	e = document.getElementById("dash1_infoWlanOpmode");
	e.innerHTML = _("dash_info wlan1 opmode");
	document.write("<td>");
	if ((nv_OP_Mode == 0) || (nv_OP_Mode == 1)){
		document.write("<span class='table_font2'><font id='dash1_infoWlanAp0'></font></span>");
		e = document.getElementById("dash1_infoWlanAp0");
		e.innerHTML = _("dash_info wlan ap0");
	}
	
	if ((nv_OP_Mode == 6) || (nv_OP_Mode == 8)){
		document.write("<span class='table_font2'><font id='dash1_infoWlanAp1'></font></span>");
		e = document.getElementById("dash1_infoWlanAp1");
		e.innerHTML = _("dash_info wlan ap1");
	}
	
	if ((nv_OP_Mode == 7) || (nv_OP_Mode == 9)){
		document.write("<span class='table_font2'><font id='dash1_infoWlanCli'></font></span>");
		e = document.getElementById("dash1_infoWlanCli");
		e.innerHTML = _("dash_info wlan wisp");
	}

	document.write("</td>");
	document.write("</tr>");
	document.write("<tr >");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanMac'></font>:</span></td>");
	e = document.getElementById("dash1_infoWlanMac");
	e.innerHTML = _("dash_info wlan mac");
	document.write("<td ><span class='table_font2'><% getWlan5GMac(); %></span></td>");
	document.write("</tr>");
	document.write("<tr >");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanStatus'></font>:</span></td>");
	e = document.getElementById("dash1_infoWlanStatus");
	e.innerHTML = _("dash_info wlan status");
	document.write("<td >");
	var wlanstatus="<% getWLAN5GLinkStatus(); %>";
	if (wlanstatus=="OFF"){
		document.write("<span class='table_font2'><font id='dash1_infoWlanStatusOff'></font></span>");
		e = document.getElementById("dash1_infoWlanStatusOff");
		e.innerHTML = _("dash_info wlan status off");
	}else{
		document.write("<span class='table_font2'><font id='dash1_infoWlanStatusOn'></font></span>");
		e = document.getElementById("dash1_infoWlanStatusOn");
		e.innerHTML = _("dash_info wlan status on");
	}
	document.write("</td>");
	document.write("</tr>");

	if ((nv_OP_Mode==0) || (nv_OP_Mode==1) || (nv_OP_Mode == 6)|| (nv_OP_Mode == 8)) {
		wlan5g_ap_channel_mode();
	} else if ((nv_OP_Mode==7) || (nv_OP_Mode==9)) {
		wlan5g_cli_channel_mode();
	}
}

function wlan24g_ap_channel_mode(){

	var ieee802_1x = "<% getCfgZero(1, 'IEEE8021X'); %>"
	var Auth = "<% getCfgZero(1, 'Auth5GMode'); %>"
	var Encry = "<% getCfgZero(1, 'Encryp5GType'); %>"
	
	//Main_SSID
	document.write("<tr >");
    document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanStatusNamessid'>(SSID)</font>:</span></td>");
    e = document.getElementById("dash_infoWlanStatusNamessid");
    e.innerHTML = _("dash_info wlan status namessid");
    document.write("<td ><span class='table_font2'><% getWLANMainSSID(); %></span></td>");
    document.write("</tr>");

	//Band_Width
	document.write("<tr >");
    document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanChanbw'></font>:</span></td>");
    e = document.getElementById("dash_infoWlanChanbw");
	e.innerHTML = _("dash_info wlan chanbw");
    document.write("<td ><span class='table_font2'><% getChannelBandWidth(); %></span></td>");
    document.write("</tr>");

    //Select_Way
    document.write("<tr >");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanChan'></font>:</span></td>");
	e = document.getElementById("dash_infoWlanChan");
    e.innerHTML = _("dash_info wlan chan");
	document.write("<td >");

	if (<% getWLANChannelSelectWay(); %> == 0 ) {
		document.write("<span class='table_font2'><font id='dash_infoWlanChanAuto'></font></span>");
		e = document.getElementById("dash_infoWlanChanAuto");
		e.innerHTML = _("dash_info wlan chan auto");
	}else {
		document.write("<span class='table_font2'><font id='dash_infoWlanChanfixed'></font></span>");
		e = document.getElementById("dash_infoWlanChanfixed");
		e.innerHTML = _("dash_info wlan chan fixed");
	}
	      			
	document.write("</td>");
	document.write("</tr>");
		  
	//OPChan
	document.write("<tr >");
    document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanOpchan'></font>:</span></td>");
    e = document.getElementById("dash_infoWlanOpchan");
    e.innerHTML = _("dash_info wlan opchan");
    document.write("<td ><span class='table_font2'><% getOpChannleASP(); %></span></td>");
    document.write("</tr>");

	//ExtChan
	document.write("<tr >");
    document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanExtchan'></font>:</span></td>");
    e = document.getElementById("dash_infoWlanExtchan");
	e.innerHTML = _("dash_info wlan extchan");
    document.write("<td ><span class='table_font2'><% getExtChannel(); %></span></td>");
    document.write("</tr>");
     
    //SecMode
    var ieee802_1x = "<% getCfgZero(1, 'IEEE8021X'); %>"
	var Auth = "<% getCfgZero(1, 'AuthMode'); %>"
	var Encry = "<% getCfgZero(1, 'EncrypType'); %>"
	
    document.write("<tr>");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanSec'></font>:</span></td>");
	e = document.getElementById("dash_infoWlanSec");
	e.innerHTML = _("dash_info wlan sec");
	document.write("<td>");

	if (ieee802_1x.charAt(0) == 1 ){
		document.write("<span class='table_font2'>802.1x</span>");
	}else if ((Auth.split(";",1) == "OPEN") && (Encry.split(";",1) == "NONE")){
		document.write("<span class='table_font2'><font id='dash_infoWlanSecNosec2'></font></span>");
		e = document.getElementById("dash_infoWlanSecNosec2");
		e.innerHTML = _("dash_info wlan sec nosec");
	}else if (Auth.split(";",1) == "SHARED"){
		document.write("<span class='table_font2'><font id='dash_infoWlanSecWEP1'></font></span>");
		e = document.getElementById("dash_infoWlanSecWEP1");
		e.innerHTML = _("dash_info wlan sec wep");
	}else if (Auth.split(";",1) == "WPAPSK"){
		document.write("<span class='table_font2'><font id='dash_infoWlanSecWPAPSK1'></font></span>");
		e = document.getElementById("dash_infoWlanSecWPAPSK1");
		e.innerHTML = _("dash_info wlan sec wpapsk");
	}else if (Auth.split(";",1) == "WPA2PSK"){
		document.write("<span class='table_font2'><font id='dash_infoWlanSecWPA2PSK1'></font></span>");
		e = document.getElementById("dash_infoWlanSecWPA2PSK1");
		e.innerHTML = _("dash_info wlan sec wpa2psk");
	}else if (Auth.split(";",1) == "WPAPSKWPA2PSK"){
		document.write("<span class='table_font2'><font id='dash_infoWlanSecWPAPSKWPA2PSK'></font></span>");
		e = document.getElementById("dash_infoWlanSecWPAPSKWPA2PSK");
		e.innerHTML = _("dash_info wlan sec wpapskwpa2psk");
	}else if (Auth.split(";",1) == "WPAWPA2"){
		document.write("<span class='table_font2'><font id='dash_infoWlanSecWPAWPA2'></font></span>");
		e = document.getElementById("dash_infoWlanSecWPAWPA2");
		e.innerHTML = _("dash_info wlan sec wpawpa2");
	}else{
		document.write("<span class='table_font2'>"+Auth.split(";",1)+"</span>");
	}
	document.write("</td>");
	document.write("</tr>");

	//wirelee_mode
	var wireless_mode = <% getWLANWirelessMode(); %>;
	
	document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlan80211mode'></font>:</span></td><td>");
	e = document.getElementById("dash_infoWlan80211mode");
    e.innerHTML = _("dash_info wlan 80211mode");
	if (wireless_mode == 0){
		document.write("<span class='table_font2'>802.11b/g</span>");
	}else if (wireless_mode == 1){
		document.write("<span class='table_font2'>802.11b</span>");
	}else if (wireless_mode == 2){
		document.write("<span class='table_font2'>802.11a</span>");
	}else if (wireless_mode == 3){
		document.write("<span class='table_font2'>802.11a/b/g</span>");
	}else if (wireless_mode == 4){
		document.write("<span class='table_font2'>802.11g</span>");
	}else if (wireless_mode == 5){
		document.write("<span class='table_font2'>802.11a/b/g/n</span>");
	}else if (wireless_mode == 6){
		document.write("<span class='table_font2'>802.11n</span>");
	}else if (wireless_mode == 7){
		document.write("<span class='table_font2'>802.11g/n</span>");
	}else if (wireless_mode == 8){
		document.write("<span class='table_font2'>802.11a/n</span>");
	}else if (wireless_mode == 9){
		document.write("<span class='table_font2'>802.11b/g/n</span>");
	}else if (wireless_mode == 10){
		document.write("<span class='table_font2'>802.11a/g/n</span>");
	}else if (wireless_mode == 11){
		document.write("<span class='table_font2'>802.11n</span>");
	}else if (wireless_mode == 14){
				document.write("<span class='table_font2'>802.11a/an/ac</span>");
	}
	document.write("</td></tr>");
}

function wlan24g_cli_channel_mode(){

	//Associated_SSID
	document.write("<tr >");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanStatusNameEssid'></font>:</span></td>");
	e = document.getElementById("dash_infoWlanStatusNameEssid");
	e.innerHTML = _("dash_info wlan associated namessid");
	var associate_status="<% getWISPapcli0ConnectStatus(); %>";
	if (associate_status=="None")
		document.write("<td ><span class='table_font2'> </span></td>");
	else {
		document.write("<td ><span class='table_font2'>");
		document.write("<% getWISPapcli0ConnectESSID(); %><% getWISPapcli0ConnectESSIDMAC(); %></span></td>");
	}
		
	document.write("</tr>");

	//Connect_Status
	document.write("<tr >");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanWISPConnectionStatus'></font>:</span></td>");
	e = document.getElementById("dash_infoWlanWISPConnectionStatus");
	e.innerHTML = _("Connect Status");	
	if (associate_status=="None")
		document.write("<td ><span class='table_font2'>"+_('Disassociated')+"</span></td>");
	if (associate_status=="Connected")
		document.write("<td ><span class='table_font2'>"+_('Associated')+"</span></td>");
	document.write("</tr>");

	//OPChan
	document.write("<tr >");
    document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanOpchan'></font>:</span></td>");
    e = document.getElementById("dash_infoWlanOpchan");
    e.innerHTML = _("dash_info wlan opchan");
    document.write("<td ><span class='table_font2'><% getOpChannleASP(); %></span></td>");
    document.write("</tr>");

	//Band_Width
	document.write("<tr >");
    document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanChanbw'></font>:</span></td>");
    e = document.getElementById("dash_infoWlanChanbw");
	e.innerHTML = _("dash_info wlan chanbw");
    document.write("<td ><span class='table_font2'><% getChannelBandWidth(); %></span></td>");
    document.write("</tr>");

	//RSSI
	if(associate_status=="Connected") {
		document.write("<tr>");
		document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanWISPRSSI'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getwifiRSSI(); %></span></td>");
		document.write("</tr>");
		e = document.getElementById("dash_infoWlanWISPRSSI");
		e.innerHTML=_("dash_info rssi");
	}

	//Sec_mode
	var Secu = <% getWISPapcli0AuthMode(); %>;
    document.write("<tr>");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanSec'></font>:</span></td>");
	e = document.getElementById("dash_infoWlanSec");
	e.innerHTML = _("dash_info wlan sec");
	document.write("<td>");
    if (Secu == 0){
        document.write("<span class='table_font2'><font id='dash_infoWlanSecNosec1'></font></span>");
        e = document.getElementById("dash_infoWlanSecNosec1");
        e.innerHTML = _("dash_info wlan sec nosec");			
    }else if (Secu == 1){
        document.write("<span class='table_font2'><font id='dash_infoWlanSecWPAPSK'></font></span>");
        e = document.getElementById("dash_infoWlanSecWPAPSK");
        e.innerHTML = _("dash_info wlan sec wpapsk");
    }else if (Secu == 2){
        document.write("<span class='table_font2'><font id='dash_infoWlanSecWPA2PSK'></font></span>");
        e = document.getElementById("dash_infoWlanSecWPA2PSK");
        e.innerHTML = _("dash_info wlan sec wpa2psk");
    }else if (Secu == 3){
        document.write("<span class='table_font2'><font id='dash_infoWlanSecWPAPSKWPA2PSK'></font></span>");
        e = document.getElementById("dash_infoWlanSecWPAPSKWPA2PSK");
        e.innerHTML = _("dash_info wlan sec wpapskwpa2psk");
    }else if (Secu == 4){
        document.write("<span class='table_font2'><font id='dash_infoWlanSecWPA'></font></span>");
        e = document.getElementById("dash_infoWlanSecWPA");
        e.innerHTML = _("dash_info wlan sec wpa");
    }else if (Secu == 5){
        document.write("<span class='table_font2'><font id='dash_infoWlanSecWPA2'></font></span>");
        e = document.getElementById("dash_infoWlanSecWPA2");
        e.innerHTML = _("dash_info wlan sec wpa2");
    }else if (Secu == 6){
        document.write("<span class='table_font2'><font id='dash_infoWlanSecWPAWPA2'></font></span>");
        e = document.getElementById("dash_infoWlanSecWPAWPA2");
        e.innerHTML = _("dash_info wlan sec wpawpa2");
    }else if (Secu == 7){
        document.write("<span class='table_font2'><font id='dash_infoWlanSecWEP'></font></span>");
        e = document.getElementById("dash_infoWlanSecWEP");
        e.innerHTML = _("dash_info wlan sec wep");
    }else if (Secu == -2) {
        document.write("<span class='table_font2'><font id='dash_infoWlanSecUnavailable'></font></span>");
        e = document.getElementById("dash_infoWlanSecUnavailable");
        e.innerHTML = _("dash_info wlan sec unavailable");
    }else if(Secu == -1) {
        document.write("<span class='table_font2'><font id='dash_infoWlanSecUnknown'></font></span>");
        e = document.getElementById("dash_infoWlanSecUnknown");
        e.innerHTML = _("dash_info wlan sec unknown");
    }else {
        document.write("<span class='table_font2'></span>");
    }
	document.write("</td></tr>");
}

function wlan24g_information() {

	var e;
	document.write("<tr>");
	document.write("<td valign='top'><span class='table_font'><font id='dash_infoWlanInfo'></font>:</span></td>");
	e = document.getElementById("dash_infoWlanInfo");
	e.innerHTML = _("dash_info wlan info");
	document.write("<td><span class='table_font2'></span></td>");
	document.write("</tr>");
	document.write("<tr>");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanOpmode'></font>: </span></td>");
	e = document.getElementById("dash_infoWlanOpmode");
	e.innerHTML = _("dash_info wlan opmode");
	document.write("<td>");
	if ((nv_OP_Mode == 0) || (nv_OP_Mode == 1)){
		document.write("<span class='table_font2'><font id='dash_infoWlanAp0'></font></span>");
		e = document.getElementById("dash_infoWlanAp0");
		e.innerHTML = _("dash_info wlan ap0");
	}
	
	if ((nv_OP_Mode == 6) || (nv_OP_Mode == 8)){
		document.write("<span class='table_font2'><font id='dash_infoWlanCli'></font></span>");
		e = document.getElementById("dash_infoWlanCli");
		
		if(addrmode == "1")
			e.innerHTML = _("dash_info wlan stawds");
		else
			e.innerHTML = _("dash_info wlan sta");
	}
	
	if ((nv_OP_Mode == 7) || (nv_OP_Mode == 9)){
		document.write("<span class='table_font2'><font id='dash_infoWlanAp1'></font></span>");
		e = document.getElementById("dash_infoWlanAp1");
		e.innerHTML = _("dash_info wlan ap1");
	}

	document.write("</td>");
	document.write("</tr>");
	
	document.write("<tr >");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanMac'></font>:</span></td>");
	e = document.getElementById("dash_infoWlanMac");
	e.innerHTML = _("dash_info wlan mac");
	document.write("<td ><span class='table_font2'><% getWlanMac(); %></span></td>");
	document.write("</tr>");

	/* //delelte ON/OFF status
	document.write("<tr >");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanStatus'></font>:</span></td>");
	e = document.getElementById("dash_infoWlanStatus");
	e.innerHTML = _("dash_info wlan status");
	document.write("<td >");
	var wlanstatus="<% getWLANLinkStatus(); %>";
	if (wlanstatus=="OFF"){
		document.write("<span class='table_font2'><font id='dash_infoWlanStatusOff'></font></span>");
		e = document.getElementById("dash_infoWlanStatusOff");
		e.innerHTML = _("dash_info wlan status off");
	}else if (wlanstatus=="ON") {
		document.write("<span class='table_font2'><font id='dash_infoWlanStatusOn'></font></span>");
		e = document.getElementById("dash_infoWlanStatusOn");
		e.innerHTML = _("dash_info wlan status on");
	} else {
		document.write("<span class='table_font2'><font id='dash_infoWlanStatusUnkown'></font></span>");
		e = document.getElementById("dash_infoWlanStatusUnkown");
		e.innerHTML = "unkown";
	}
	document.write("</td>");
	document.write("</tr>");
	*/

	if ((nv_OP_Mode==0) || (nv_OP_Mode==1) || (nv_OP_Mode == 7)|| (nv_OP_Mode == 9)) {
		wlan24g_ap_channel_mode();
	} else if ((nv_OP_Mode==6) || (nv_OP_Mode==8)) {
		wlan24g_cli_channel_mode();
	}
}
</script>
</head>
<body onload="initTranslation();">
<form method="post" name="NTP" action="/goform/NTP">
<div id="popup"></div>
<div id="contentframe">
  <div>
    <table width="100%" border="0" cellspacing="0" cellpadding="0"><tr>
        <td width="1" bgcolor="#779bc8"><img src="../images/air.gif" width="1" height="38" /></td>
        <td width="100%"  valign="top" bgcolor="#39639e">
        <div class="pannel2">
		<div class="leftpannel">

        <div class="w_text3">
   <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="table_frame">
     <tr>
       <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
         <tr>
           <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
             <tr>
               <td><table width="100%" cellpadding="0" cellspacing="0" >
                 <tr>
                   <td height="26" colspan="2"><table width="100%" border="0" cellspacing="0" cellpadding="0">
                       <tr>
                         <td height="26" background="images/table_top_center.gif" class="table_title"><font id="dash_infoTitle"></font></td>
                       </tr>
                   </table></td>
                 </tr>
                 <tr >
                   <td width="60%" valign="top"><span class="top_font"><font id="dash_infoItem"></font></span></td>
                   <td width="50%"><span class="top_font"><font id="dash_infoData"></font></span></td>
                 </tr>
                 <tr >
                   <td valign="top"><span class="table_font"><font id="dash_infoProductName"></font>:</span></td>
                   <td ><span class="table_font2"><% getCfgGeneral(1, 'ProductName'); %></span></td>
                 </tr>
                 <tr >
                   <td valign="top"><span class="table_font"><font id="dash_infoHostName"></font>:</span></td>
                   <td ><span class="table_font2"><% getCfgGeneral(1, 'SystemName'); %></span></td>
                 </tr>

                 <tr >
                   <td valign="top"><span class="table_font"><font id="dash_infoFwVer"></font>: </span></td>
                   <td ><span class="table_font2"><% getFirmwareVersion(); %></span></td>
                 </tr>
                 <tr >
                   <td valign="top"><span class="table_font"><font id="dash_infoSysOpmode"></font>: </span></td>
                   <td >
<script>
	var e;
	if (nv_OP_Mode == 0){
		document.write("<span class='table_font2'><font id='dash_infoSysRouter'></font></span>");
		e = document.getElementById("dash_infoSysRouter");
		e.innerHTML = _("dash_info sys router");
	}
	
	if (nv_OP_Mode == 1){
		document.write("<span class='table_font2'><font id='dash_infoSysAp'></font></span>");
		e = document.getElementById("dash_infoSysAp");
		e.innerHTML = _("dash_info sys ap");
	}

	if (nv_OP_Mode == 2){
		document.write("<span class='table_font2'><font id='dash_infoSysClient'></font></span>");
		e = document.getElementById("dash_infoSysClient");
		e.innerHTML = _("dash_info sys clinet");
	}

	if (nv_OP_Mode == 3){
		document.write("<span class='table_font2'><font id='dash_infoSysWds'></font></span>");
		e = document.getElementById("dash_infoSysWds");
		e.innerHTML = _("dash_info sys wds");
	}

	if (nv_OP_Mode == 4){
		document.write("<span class='table_font2'><font id='dash_infoSysApwds'></font></span>");
		e = document.getElementById("dash_infoSysApwds");
		e.innerHTML = _("dash_info sys apwds");
	}

	if (nv_OP_Mode == 5){
		document.write("<span class='table_font2'><font id='dash_infoSysApcli'></font></span>");
		e = document.getElementById("dash_infoSysApcli");
		e.innerHTML = _("dash_info sys apcli");
	}

	if (nv_OP_Mode == 6){
		document.write("<span class='table_font2'><font id='dash_infoSysWisp'></font></span>");
		e = document.getElementById("dash_infoSysWisp");
		e.innerHTML = _("dash_info sys wisp");
	}
	
	if (nv_OP_Mode == 7){
		document.write("<span class='table_font2'><font id='dash_infoSysWispUR'></font></span>");
		e = document.getElementById("dash_infoSysWispUR");
		e.innerHTML = _("dash_info sys wisp");
	}

	if (nv_OP_Mode == 8){
		document.write("<span class='table_font2'><font id='dash_infoSysClient'></font></span>");
		e = document.getElementById("dash_infoSysClient");
		e.innerHTML = _("dash_info sys clinet");
	}

	if (nv_OP_Mode == 9){
		document.write("<span class='table_font2'><font id='dash_infoSysClient'></font></span>");
		e = document.getElementById("dash_infoSysClient");
		e.innerHTML = _("dash_info sys clinet");
	}
</script>
</td>
</tr>

<script>                              
	if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6)||(nv_OP_Mode == 7)){
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoWanInfo'></font> :</span></td>");
		document.write("<td ><span class='table_font2'></span></td></tr>");
 		
		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;  - <font id='dash_infoWanMac'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getWanMac(); %></span></td></tr>");
		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWanIp'></font>: </span></td>");
		document.write("<td ><span class='table_font2'><% getWanIp(); %></span></td></tr>");
		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWanSubmask'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getWanNetmask(); %></span></td></tr>");
		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWanGateway'>"+_('g2-2 Gateway1')+"</font>:</span></td>");
		var wan_gateway="<% getWanGateway(); %>";
		if (wan_gateway.length==0)
			document.write("<td ><span class='table_font2'>0.0.0.0</span></td></tr>");
		else	
			document.write("<td ><span class='table_font2'>"+wan_gateway+"</span></td></tr>");
	
		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWanContype'>Connected Type</font>:</span></td>");
		var wan_type="<% getCfgGeneral(1, "wanConnectionMode"); %>";
		if (wan_type == "STATIC" )
			document.write("<td valign='top'><span class='table_font2'>"+_("g2-1 InternetType1")+"</span></td></tr>");
		else if(wan_type == "DHCP")
			document.write("<td valign='top'><span class='table_font2'>"+_("g2-1 InternetType2")+"</span></td></tr>");
		else if(wan_type == "PPPOE")
			document.write("<td valign='top'><span class='table_font2'>"+_("g2-1 InternetType3")+"</span></td></tr>");
		else if(wan_type == "PPTP")
			document.write("<td valign='top'><span class='table_font2'>"+_("g2-1 InternetType4")+"</span></td></tr>");
		else if(wan_type == "L2TP")
			document.write("<td valign='top'><span class='table_font2'>"+_("g2-1 InternetType5")+"</span></td></tr>");
		else
			document.write("<td valign='top'><span class='table_font2'>"+_("dash ref none")+"</span></td></tr>");
 		                 
	}                 
</script>
<tr >
  <td valign="top"><span class="table_font"><font id="dash_infoLanInfo"></font>:</span></td>
  <td ><span class="table_font2"></span></td>
</tr>
<tr >
  <td valign="top"><span class="table_font">&nbsp;&nbsp;- <font id="dash_infoLanMac"></font>:</span></td>
  <td ><span class="table_font2"><% getLanMac(); %></span></td>
</tr>
<tr >
  <td valign="top"><span class="table_font">&nbsp;&nbsp;- <font id="dash_infoLanIp"></font>: </span></td>
  <td ><span class="table_font2"><% getLanIp(); %></span></td>
</tr>
<tr >
  <td valign="top"><span class="table_font">&nbsp;&nbsp;- <font id="dash_infoLanSubmask"></font>:</span></td>
  <td ><span class="table_font2"><% getLanNetmask(); %></span></td>
</tr>
<script>
	if ((nv_OP_Mode == 1) || (nv_OP_Mode == 8) || (nv_OP_Mode == 9)){
		var lan_gw="<% getLanIfGateway(); %>";
		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoLanGateway'>"+_('g2-2 Gateway1')+"</font>:</span></td>");
		document.write("<td ><span class='table_font2'>");
		if (lan_gw.length==0)
			document.write("0.0.0.0");
		else	
			document.write(lan_gw);
		document.write("</span></td></tr>");
	}
</script>

<script>
	var e;
	if ((nv_OP_Mode == 0) || (nv_OP_Mode == 6) || (nv_OP_Mode == 7)){
		var DHCPData = '<% getCfgZero(1, "dhcpMode"); %>';
		
		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoLanDhcpServer'></font>:</span></td>");
		e = document.getElementById("dash_infoLanDhcpServer");
		e.innerHTML = _("dash_info lan dhcp server");
		
		if (DHCPData=="DHCPS"){
			document.write("<td ><span class='table_font2'><font id='dash_infoLanDhcpSEna'></font></span>");
			e = document.getElementById("dash_infoLanDhcpSEna");
			e.innerHTML = _("dash_info lan dhcp ena");
		}else{
			document.write("<td ><span class='table_font2'><font id='dash_infoLanDhcpSDis'>"+_("dash_info lan dhcp dis")+"</font></span>");
			e = document.getElementById("dash_infoLanDhcpSDis");
			e.innerHTML = _("dash_info lan dhcp dis");
		}
		document.write("</td></tr>");

		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoLanDhcpRelay'></font>:</span></td>");
		e = document.getElementById("dash_infoLanDhcpRelay");
		e.innerHTML = _("dash_info lan dhcp relay");
		
		if (DHCPData=="RELAY"){
			document.write("<td ><span class='table_font2'><font id='dash_infoLanDhcpREna'></font></span>");
			e = document.getElementById("dash_infoLanDhcpREna");
			e.innerHTML = _("dash_info lan dhcp ena");
		}else{
			document.write("<td ><span class='table_font2'><font id='dash_infoLanDhcpRDis'>"+_("dash_info lan dhcp dis")+"</font></span>");
			e = document.getElementById("dash_infoLanDhcpRDis");
			e.innerHTML = _("dash_info lan dhcp dis");
		}
		document.write("</td></tr>");
	}

	if ((nv_OP_Mode == 1) || (nv_OP_Mode == 5) || (nv_OP_Mode == 8) || (nv_OP_Mode == 9)){
		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoLanDhcpClient'></font>:</span></td>");
		e = document.getElementById("dash_infoLanDhcpClient");
		e.innerHTML = _("dash_info lan dhcp client");
		
		var lanIpmode = '<% getCfgZero(1, "lanIp_mode"); %>';
		if (lanIpmode=="0"){//DHCP
			document.write("<td ><span class='table_font2'><font id=ap_dhcp>"+_("Enable")+"</font></span>");
		}else{
			document.write("<td ><span class='table_font2'><font id=ap_dhcp>"+_("Disable")+"</font></span>");
		}
		document.write("</td></tr>");
	}
</script>

<script>
if ((board_type == "WP688") || (board_type == "WP838"))
{
	var wlanstatus="<% getWLANLinkStatus(); %>";
	if (wlanstatus=="ON") {
		wlan24g_information();
	}
}
</script>

<script>
if (board_type == "WP838")
{
	var wlanstatus="<% getWLAN5GLinkStatus(); %>";
	if (wlanstatus=="ON") {
		wlan5g_information();
	}
}
</script>

</table></td>
</tr>
</table></td>
</tr>
</table></td>
</tr>
</table>
</div>
<script>
if (board_type == "WP838")
{
	document.write("<br/>");
	document.write("<br/>");
}
</script>
 
		  <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
              <td></td>
            </tr>
          </table>
		</div>
		<div class="rightpannel">

		  <div class="w_text3">
		    <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="table_frame">
              <tr>
                <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                          <tr>
                            <td><table width="100%" cellpadding="0" cellspacing="0" >
                                <tr>
                                  <td height="26" colspan="2"><table width="100%" border="0" cellspacing="0" cellpadding="0">
                                      <tr>
                                        <td height="26" background="images/table_top_center.gif" class="table_title"><font id="dash_infoSystatTitle"></font></td>
                                      </tr>
                                  </table></td>
                                </tr>
                                <tr >
                                  <td width="60%" valign="top"><span class="top_font"><font id="dash_infoSystatItem"></font></span></td>
                                  <td width="50%"><span class="top_font"><font id="dash_infoSystatData"></font></span></td>
                                </tr>
                                <tr >
                                  <td valign="top"><span class="table_font"><font id="dash_infoSystatSysuptime"></font>:</span></td>
                                  <td ><span class="table_font2"><% getSysUptime(); %></span></td>
                                </tr>
                                <tr >
                                  <td valign="top"><span class="table_font"><font id="dash_infoSystatCurrtime"></font>:</span></td>
                                  <td><span class="table_font2"><a id="mtenCurrent_Date"></a>&nbsp/&nbsp<a id="mtenCurrent_Time"></a></span></td>
                                  <input type="hidden" name="mtenCurrent_Hour" value="<% getCurrentHour();%>" />
                                  <input type="hidden" name="mtenCurrent_Min" value="<% getCurrentMin();%>" />
                                  <input type="hidden" name="mtenCurrent_Sec" value="<% getCurrentSec();%>" />
                                  <input type="hidden" name="mtenCurrent_Year" value="<% getCurrentYear();%>" />
                                  <input type="hidden" name="mtenCurrent_Mon" value="<% getCurrentMon();%>" />
                                  <input type="hidden" name="mtenCurrent_Day" value="<% getCurrentDay();%>" />
                                </tr>
                                <tr >
                                  <td valign="top"><span class="table_font"><font id="dash_infoSystatSysres"></font>:</span></td>
                                  <td ><span class="table_font2"></span></td>
                                </tr>
<% getSysResource(); %>
                               
<script>
if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6)||(nv_OP_Mode == 7)){
	document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoSystatSettingFw'></font>:</span></td>");
	document.write("<td >");
	if (<% getCfgZero(1, "FIREWALLEnabled"); %>){
		document.write("<span class='table_font2'><font id='dash_infoSystatSettingEn'></font></span>");
		e = document.getElementById("dash_infoSystatSettingEn");
		e.innerHTML = _("dash_info systat setting en");
	}else{
		document.write("<span class='table_font2'><font id='dash_infoSystatSettingDis'></font></span>");
		e = document.getElementById("dash_infoSystatSettingDis");
		e.innerHTML = _("dash_info systat setting dis");
	}
	document.write("</td></tr>");
	//document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoSystatBm'></font>: </span></td>");
	//document.write("<td >");
	//if (<% getCfgZero(1, "RL_QoSEnable"); %>){
	//	document.write("<span class='table_font2'><font id='dash_infoSystatBmEn'></font></span>");
	//	e = document.getElementById("dash_infoSystatBmEn");
	//	e.innerHTML = _("dash_info systat bm en");
	//}else{
	//	document.write("<span class='table_font2'><font id='dash_infoSystatBmDis'></font></span>");
	//	e = document.getElementById("dash_infoSystatBmDis");
	//	e.innerHTML = _("dash_info systat bm dis");
	//}
	//document.write("</td></tr>");
	document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- UPnP:</span></td>");
	document.write("<td >");
	if (<% getCfgZero(1, "upnpEnabled"); %>){
		document.write("<span class='table_font2'><font id='dash_infoSystatUpnpEn'></font></span>");
		e = document.getElementById("dash_infoSystatUpnpEn");
		e.innerHTML = _("dash_info systat upnp en");
	}else{
		document.write("<span class='table_font2'><font id='dash_infoSystatUpnpDis'></font></span>");
		e = document.getElementById("dash_infoSystatUpnpDis");
		e.innerHTML = _("dash_info systat upnp dis");
	}
	document.write("</td></tr>");
}
</script>
                                
                            </table></td>
                          </tr>
                      </table></td>
                    </tr>
                </table></td>
              </tr>
            </table>
		  </div>
		  <br />
		  <br />
<script>
/* //Deleted by Andy Yu in 2013/07/11: IPV6
if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6)||(nv_OP_Mode == 7)){
        document.write('<div class="w_text3">');
   		document.write('<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="table_frame">');
     	document.write('<tr>');
       	document.write('<td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">');
        document.write('<tr>');
        document.write('<td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">');
        document.write('<tr>');
        document.write('<td><table width="100%" cellpadding="0" cellspacing="0" >');
        document.write('<tr>');
        document.write('<td height="26" colspan="2"><table width="100%" border="0" cellspacing="0" cellpadding="0">');
        document.write('<tr>');
        document.write('<td height="26" background="images/table_top_center.gif" class="table_title"><font id="dash_infoIPv6Title"></font></td>');
        document.write('</tr>');
        document.write('</table></td>');
        document.write('</tr>');
        document.write('<tr >');
        document.write('<td width="45%" valign="top"><span class="top_font"><font id="dash_infoIPv6Item"></font></span></td>');
        document.write('<td width="55%"><span class="top_font"><font id="dash_infoIPv6Data"></font></span></td>');
        document.write('</tr>');
       
////  IPv6 Connection Status    
	   
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6ConnType'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getIPv6ConnType(); %></span></td></tr>");
if((wan_ipv6_proto == '0') || (wan_ipv6_proto == '1')){ 
	
	if(wan_ipv6_proto == '0'){
		//IPv6 Static WAN Adrress
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6WanAddress'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getCfgGeneral(1, "wan_ipv6addr"); %>/<% getCfgGeneral(1, "wan_ipv6netsize"); %></span></td></tr>");
	
		//IPv6 Static WAN GATEWAY
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6WanGateway'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getCfgGeneral(1, "wan_ipv6gateway"); %>/<% getCfgGeneral(1, "wan_ipv6netsize"); %></span></td></tr>");
		
		//IPv6 Static WAN 1 dns
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6WanFirstDns'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getIPv6StaticDns(1); %></span></td></tr>");
	
		//IPv6 Static WAN 2 dns
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6WanSecondDns'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getIPv6StaticDns(2); %></span></td></tr>");
	}else if(wan_ipv6_proto == '1'){
		//IPv6 WAN Adrress
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6WanAddress'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getIPv6DhcpWanAddr(); %></span></td></tr>");
	
		//IPv6 WAN GATEWAY
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6WanGateway'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getIPv6DhcpWanDateway(); %></span></td></tr>");
		
		//IPv6 DHCP WAN 1 dns
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6WanFirstDns'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getIPv6DHCPDns(1); %></span></td></tr>");
	
		//IPv6 DHCP WAN 2 dns
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6WanSecondDns'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getIPv6DHCPDns(2); %></span></td></tr>");
	}
		//IPv6 LAN Address
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6LanAddress'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getIPv6LanAddr(); %></span></td></tr>");
}
		//IPv6 Link Local
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6LanLinkLocal'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getLinkLocalAddr(); %></span></td></tr>");


		document.write('</table></td>');
		document.write('</tr>');
		document.write('</table></td>');
		document.write('</tr>');
		document.write('</table></td>');
		document.write('</tr>');
		document.write('</table>');
		document.write('</div>');
		document.write('<br />');
		document.write('<br />');
}
*/
</script>
  		  
		  <div class="w_text3">
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="table_frame">
              <tr>
                <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                          <tr>
                            <td><table width="100%" cellpadding="0" cellspacing="0" >
                              <tr>
                                <td height="26"><table width="100%" border="0" cellspacing="0" cellpadding="0">
                                    <tr>
                                      <td height="26" background="images/table_top_center.gif" class="table_title"><font id="dash_infoSumTitle"></font></td>
                                    </tr>
                                </table></td>
                              </tr>
<script>                              
if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6)||(nv_OP_Mode == 7)){
	//document.write("<tr ><td width='60%' valign='top'><span class='table_font'><font id='dash_infoSumBw'></font> <a target='_parent' href='/local/advance/main_monitor_frame.asp?1'>(<font id='dash_infoSumBwDet'>Details</font>...)</a></span></td></tr>");
	document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoSumDhcp'>DHCP Table</font> <a target='_parent' href='/local/advance/main_monitor_frame.asp?1'>(<font id='dash_infoSumDhcpDet'>Details</font>...)</a></span></td></tr>");
}
</script>                            
<tr >
<td valign="top"><span class="table_font"><font id="dash_infoSumPktstat">Packet Statistics</font> <a target='_parent' href="/local/advance/main_monitor_frame.asp?2">(<font id="dash_infoSumPktstatDet">Details</font>...)</a></span></td>
</tr>
<script>                              
if ((nv_OP_Mode == 0)||(nv_OP_Mode == 1)||(nv_OP_Mode == 7)||(nv_OP_Mode == 9)){
	document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoSumWlansta'>WLAN Station Status</font> <a target='_parent' href='/local/advance/main_monitor_frame.asp?3'>(<font id='dash_infoSumWlanstaDet'>Details</font>...)</a></span></td></tr>");
}
//CPE_5G don't display
if ((board_type == "WP838")&&((nv_OP_Mode == 0)||(nv_OP_Mode == 1)||(nv_OP_Mode == 6)||(nv_OP_Mode == 8))){
	document.write("<tr style='display:none'><td valign='top'><span class='table_font'><font id='dash_infoSumWlan1sta'>WLAN5G Station Status</font> <a target='_parent' href='/local/advance/main_monitor_frame.asp?4'>(<font id='dash_infoSumWlan1staDet'>Details</font>...)</a></span></td></tr>");
}
</script>
                              
                            </table></td>
                          </tr>
                      </table></td>
                    </tr>
                </table></td>
              </tr>
            </table>
		    </div>
		  </div>
		</div>
            <br class="clearfloat" />
            <br />
          <br />
        </td>
        <td width="1" bgcolor="#779bc8"><img src="../images/air.gif" width="1" height="38" /></td>
      </tr>
    </table>
  </div>
</div>
</form>
</body>
<script>
 start_Mon="<% getCfgGeneral(1, "startMon"); %>";
 start_Day="<% getCfgGeneral(1, "startDay"); %>";
 start_Hour="<% getCfgGeneral(1, "startclock"); %>";
 end_Mon="<% getCfgGeneral(1, "endMon"); %>";
 end_Day="<% getCfgGeneral(1, "endDay"); %>";
 end_Hour="<% getCfgGeneral(1, "endclock"); %>";
 enable_dst = "<% getCfgGeneral(1, "enabledaylight"); %>";

	if(enable_dst=="1")
	{
	daylightsaving();
	}
initDateAndTime();
IncreaseSec();
</script>
<script>
parent.change_iframe_height(document.body.scrollHeight);
</script>
</html>
