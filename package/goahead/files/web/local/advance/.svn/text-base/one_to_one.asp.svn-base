<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>

<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("firewall");
var lanIP = "<%getLanIp(); %>"; 
var wanIP = "<%getWanIp(); %>"; 

function initTranslation(){
	var e = document.getElementById("OneToOneRuleAdd");
	e.innerHTML = _("OneToOne Rule Add");
	e = document.getElementById("OneToOneActive");
	e.innerHTML = _("OneToOne Active");		
	e = document.getElementById("OneToOneServiceName");
	e.innerHTML = _("OneToOne Service Name");
	e = document.getElementById("OneToOnePort");
	e.innerHTML = _("OneToOne Port");
	
	e = document.getElementById("InternalIP");
	e.innerHTML = _("Internal IP");
	e = document.getElementById("ExternalIP");
	e.innerHTML = _("External IP");
	e = document.getElementById("ServerForward");
	e.innerHTML = _("Server Forward");
	e = document.getElementById("ForwardEnable");
	e.innerHTML = _("Forward Enable");
	e = document.getElementById("ForwardDisable");
	e.innerHTML = _("Forward Disable");
	e = document.getElementById("OneToOneAppRuleSum");
	e.innerHTML = _("OneToOne App Rule Sum");		
	e = document.getElementById("OneToOneAppRuleSum2");
	e.innerHTML = _("OneToOne App Rule Sum2");	
	e = document.getElementById("OneToOneTdActive");
	e.innerHTML = _("OneToOne Td Active");
	e = document.getElementById("OneToOneTdName");
	e.innerHTML = _("OneToOne Td Name");
	e = document.getElementById("OneToOneTdIntIP");
	e.innerHTML = _("OneToOne Td IntIP");
	e = document.getElementById("OneToOneTdExtIP");
	e.innerHTML = _("OneToOne Td ExtIP");	
	e = document.getElementById("OneToOneTdPort");
	e.innerHTML = _("OneToOne Td Port");
	e = document.getElementById("OneToOneTdServerForward");
	e.innerHTML = _("OneToOne Td ServerForward");
	e = document.getElementById("OneToOneTdModify");
	e.innerHTML = _("OneToOne Td Modify");
	e = document.getElementById("natApply");
	e.value = _("nat apply");
	e = document.getElementById("natReset");
	e.value = _("nat reset");
	e = document.getElementById("User_Defined");
	e.innerHTML = _("User_Defined");
	e = document.getElementById("Services");
	e.innerHTML = _("Services");
	e = document.getElementById("AlertNote");
	e.innerHTML = _("Alert note");
	e = document.getElementById("oneNote");
	e.innerHTML = _("one note");
	e = document.getElementById("one2oneNote");
	e.innerHTML = _("one2one note");
}

<!--
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
function checkSubnet(ip, mask, client)
{ 
  ip_d = atoi(ip, 1);
  mask_d = atoi(mask, 1);
  client_d = atoi(client, 1);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
	return false;

  ip_d = atoi(ip, 2);
  mask_d = atoi(mask, 2);
  client_d = atoi(client, 2);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
	return false;

  ip_d = atoi(ip, 3);
  mask_d = atoi(mask, 3);
  client_d = atoi(client, 3);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
	return false;

  ip_d = atoi(ip, 4);
  mask_d = atoi(mask, 4);
  client_d = atoi(client, 4);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
	return false;

  return true;
}
function doServiceChange(index){
    document.NAT_APP.PortRule_Active.checked = true;    
	document.NAT_APP.PortRule_Active.value = "1";
	if((index == 1) || (index == 9)){
	    document.NAT_APP.PortRule_Name.value = "";
	    document.NAT_APP.PortRule_Port.value = "";
		return;
	}else{
		if(index==2){	
			document.NAT_APP.PortRule_Name.value = "DNS";
			document.NAT_APP.PortRule_Port.value = "53";			
		}else if(index==3){
			document.NAT_APP.PortRule_Name.value = "FTP";
			document.NAT_APP.PortRule_Port.value = "21";
		}
		else if(index==4){
			document.NAT_APP.PortRule_Name.value = "HTTP";
			document.NAT_APP.PortRule_Port.value = "80";
		}
		else if(index==5){
			document.NAT_APP.PortRule_Name.value = "HTTPS";
			document.NAT_APP.PortRule_Port.value = "443";
		}
		else if(index==6){
			document.NAT_APP.PortRule_Name.value = "POP3";
			document.NAT_APP.PortRule_Port.value = "110";
		}
		else if(index==7){
			document.NAT_APP.PortRule_Name.value = "SMTP";
			document.NAT_APP.PortRule_Port.value = "25";
		}
		else if(index==8){
			document.NAT_APP.PortRule_Name.value = "TELNET";
			document.NAT_APP.PortRule_Port.value = "23";
		}
		else{
			document.NAT_APP.PortRule_Name.value = "";
			document.NAT_APP.PortRule_Port.value = "";
		}				
		return;
	}
}

function natClick(){
	document.NAT_APP.PortRule_Active.value = document.NAT_APP.PortRule_Active.checked ? "1": "0";	
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

function checkRange(str, num, min, max)
{
        var k = 0;
	 for (var i=0; i<str.length; i++) {
		if (str.charAt(i) == '.')
			k = k+1;
		continue;
	}
	if(k > 3){
             return false;
	}
	d = atoi(str, num);
	if (d > max || d < min)
		return false;
	return true;
}


function check127net(str, num)
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
	if (d ==127 )
	{
	       alert("Error. IP address is loopback address.");
		return true;
	}
      
	return false;
}

function checkmultinet(str, num)
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
	if (d >223 && d <240 )
	{
	      alert("Error. IP address is multicast address.");
		return true;
	}      
	return false;
}


function checkIpAddr(field, ismask)
{
	if (isAllNum(field.value) == 0) {
		alert('It should be a [0-9] number.');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
      if (check127net(field.value,1) == true) {
		field.focus();
		return false;
	}

	if (checkmultinet(field.value,1) == true) {
		field.focus();
		return false;
	}
	
	//aron modify to limit max to 255
	if (ismask) {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 0, 255)))
		{
			alert('IP adress format error.');
			//field.value = field.defaultValue;
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
			alert('IP adress format error.');
			//field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	return true;
}

function checkDuplicatePortRange(p1, p2) {
	var port1 = new Array();
	var port2 = new Array();                             
	var port1_start;                       
	var port1_end;                                   
	var port2_start;                                  
	var port2_end;                                        
	port1 = p1.split("-");                     
	port2 = p2.split("-");                        
	port1_start = parseInt(port1[0],10);           
	port1_end = parseInt(port1[1],10);            
	port2_start = parseInt(port2[0],10);               
	port2_end = parseInt(port2[1],10);
	if(!(port1.length))
		return 1;
	if(port1.length == 1) {
		if(port2.length ==1) {
			return (port1_start == port2_start) ? 1 : 0;
		} else {
			return (port1_start>=port2_start && port1_start<=port2_end) ? 1 : 0;
		}
	} else {
		if(port2.length ==1) {
			return (port2_start>=port1_start && port2_start<=port1_end) ? 1 : 0;
		} else {
			if((port1_start>=port2_start && port1_start<=port2_end) 
				|| (port1_end>=port2_start && port1_end<=port2_end)
				|| (port1_start<=port2_start && port1_end>=port2_end))
				return 1;
			else 
				return 0;
		}
	}

}

function checkDupliPort(){
	var num = "<% getCfgGeneral(1, "one_to_one_num"); %>";		
	if(num == 0)
		return true;
	else{
		var port_str="<% getCfgGeneral(1, "forward_port"); %>";
		var entries = new Array();
		if(port_str.length) {
			entries = port_str.split(";");
			for(i=0; i<entries.length; i++){
				if(checkDuplicatePortRange(document.NAT_APP.PortRule_Port.value,entries[i]))
					return false;
				else
					continue;
			}
			return true;
		}
	}
}
/*
add by wind 2013-12-18
judge the rules situation ,including internal ipaddr,external ipaddr,port*/
/*check duplicated rules,add by wind 2013-12-18*/
function judgeStu(intip,extip){
	if(document.NAT_APP.PortRule_ExtAddr.value == extip)
		return 1;
	else if(document.NAT_APP.PortRule_IntAddr.value != intip)
		return 2;
	else
		return 3;
}
function checkDuplirule(){
	var num = "<% getCfgGeneral(1, "one_to_one_num"); %>";		
	if(num == 0)
		return true;
	else{
		var port_str="<% getCfgGeneral(1, "forward_port"); %>";
		var intipaddr="<% getCfgGeneral(1, "inter_ipaddr"); %>";
		var extipaddr="<% getCfgGeneral(1, "exter_ipaddr"); %>";
		var entries = new Array();
		var entries_intipaddr = new Array();
		var entries_extipaddr = new Array();
		if(intipaddr.length) {
			entries = port_str.split(";");
			entries_intipaddr = intipaddr.split(";");
			entries_extipaddr = extipaddr.split(";");
			for(i=0; i<entries_intipaddr.length; i++){
				var stu = judgeStu(entries_intipaddr[i],entries_extipaddr[i]);
				switch(stu){
					case 1: 
						if(!entries[i])
							return false;
						else if(checkDuplicatePortRange(document.NAT_APP.PortRule_Port.value,entries[i]))
							return false;
						break;
					case 2:
						break;
					case 3:
						return false;
							
				}
				
			}
			return true;
		}
	}
}
function editCheckDuplirule(){
	var num = "<% getCfgGeneral(1, "one_to_one_num"); %>";
	num = parseInt(num,10)
	if(num == 1)
		return true;
	else{
		var port_str="<% getCfgGeneral(1, "forward_port"); %>";
		var intipaddr="<% getCfgGeneral(1, "inter_ipaddr"); %>";
		var extipaddr="<% getCfgGeneral(1, "exter_ipaddr"); %>";
		var entries = new Array();
		var entries_intipaddr = new Array();
		var entries_extipaddr = new Array();
		var index = "<% getCfg_OneToOneNat(1, "edit_index"); %>";
		var indexNum = parseInt(index,10);
		if(intipaddr.length) {
			entries = port_str.split(";");
			entries_intipaddr = intipaddr.split(";");
			entries_extipaddr = extipaddr.split(";");
			
			for(i=0; i<entries_intipaddr.length; i++){
				if((i == indexNum-1) || i == num-1){
					if(document.NAT_APP.PortRule_Port.value == entries[i] && document.NAT_APP.PortRule_ExtAddr.value == entries_extipaddr[i] && document.NAT_APP.PortRule_IntAddr.value == entries_intipaddr[i])
						break;
					else
						continue;
				}
				var stu = judgeStu(entries_intipaddr[i],entries_extipaddr[i]);
				switch(stu){
					case 1: 
						if(!entries[i])
							return false;
						else if(checkDuplicatePortRange(document.NAT_APP.PortRule_Port.value,entries[i]))
							return false;
						break;
					case 2:
						break;
					case 3:
						return false;
							
				}
				
			}
			return true;
		}
	}
}
function checkTableNum()
{
	
	var num = "<% getCfgGeneral(1, "one_to_one_num"); %>";		
	var max = "<% getCfgGeneral(1, "one_to_one_max"); %>";
	num = parseInt(num,10);
	max = parseInt(max,10);
	if(num >= max){
	   alert('The rule number is exceeded.');
	   return false;
	}
	else
	   return true; 
}
//Steve add
function isPortRule(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == ',' ) || (str.charAt(i) == '-' ))
			continue;
		return 0;
	}
	return 1;
}
function getValueCount(str)
{
	var num=0;
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) == ',' )  || (str.charAt(i) == '-' ))
			num++;
	}
	return num;
}
function getValue2Count(str,char1)
{
	var num=0;
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) == char1 ))
			num++;
	}
	return num;
}

function getNthValue(str,index,char1)
{
	var num=0;
	var s1=0;
	var s2=0;
	var max=0;

	max=getValue2Count(str,char1);
	
if(index == 1){
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) == char1 ))
			num++;		
		s1=0;		
		if(num == index){
		   		s2=i;
		   		break;
		   	}
	}
}else if(index == max+1){
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) == char1 ))
			num++;		
		s2=str.length;		
		if(num == max){
		   		s1=i+1;
		   		break;
		   	}
	}

}else{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) == char1 ))
			num++;
			
		if(num == (index-1)){
		   		s1=i+1;
		   		break;
		   	}
	}
	num=0;
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) == char1 ))
			num++;
			
		if(num == index){
		   		s2=i;
		   		break;
		   	}
	}
}
	str1=str.substring(s1,s2);
    return str1;
}

function checkPortRule(field)
{
    var len=0;
    
    //check port value if null
	/*if(field.value == ""){
		alert("Please set port between 1~65535");
		return false;
	}*/
	
	//Rule accept "0~9" & "," & "-"
	if (isPortRule(field.value) == 0) {
		alert('Invaild setting');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	//The sum of "," and  "-" is less than 15
	if (getValueCount(field.value) >= 15) {
		alert('Invaild setting');
		field.value = field.defaultValue;
		field.focus();
		    return false;
	}
	
	//check last char
	len=field.value.length;
	if ((field.value.charAt(len-1) == ',' )  || (field.value.charAt(len-1) == '-' )){
		alert('Invaild setting');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
	
	
	//check port range
	if (getValueCount(field.value) == 0) {
		if(field.value != ""){
		d1 = atoi(field.value, 1);
		if(isAllNum( field.value ) == 0){
			alert("Please set port between 1~65535");
			field.focus();
			return false;
		}
		if(d1 > 65535 || d1 < 1){
			alert("Please set port between 1~65535");
			field.focus();
			return false;
		}			
	}
	}else{
	    var max=0;
		max=getValue2Count(field.value,',');
		if(max == 0){
		    str1=field.value;
			var num=0;
		    len=str1.length;
		    num=getValue2Count(str1,'-');
		    if((num >= 2) || (len <= 2)){
		    	alert("Invaild setting");field.focus();return false;
		    }else{
		    	t1=str1;
		    	t2=str1;
		    	s1=getNthValue(t1,1,'-');
		    	s2=getNthValue(t2,2,'-');
		    	
		    	d1 = atoi(s1, 1);
		    	d2 = atoi(s2, 1);
		    	if(d1 > 65535 || d1 < 1){
		    		alert("Please set port between 1~65535");field.focus();return false;
		    	}
		    	if(d2 > 65535 || d2 < 1){
		    		alert("Please set port between 1~65535");field.focus();return false;
		    	}
		    	if(d1 >= d2){
		    		alert("Invaild setting");field.focus();return false;
		    	}
		    }
		}else{
			for (var i=1; i < max+2; i++) {
	   			str1=getNthValue(field.value,i,',');
	   			if(str1 == ""){
		    		alert("Please set port between 1~65535");field.focus();return false;
		    	}
		    	if(str1.indexOf("-",0) == -1){
		    		d1 = atoi(str1, 1);
		    		if(d1 > 65535 || d1 < 1){
		    			alert("Please set port between 1~65535");field.focus();return false;
		    		}
		    	}else{
		    		var num=0;
		    		len=str1.length;
		    		num=getValue2Count(str1,'-');
		    		if((num >= 2) || (len <= 2)){
		    			alert("Invaild setting");field.focus();return false;
		    		}else{
		    		    t1=str1;
		    		    t2=str1;
		    			s1=getNthValue(t1,1,'-');
		    			s2=getNthValue(t2,2,'-');
		    			d1 = atoi(s1, 1);
		    			d2 = atoi(s2, 1);
		    			if(d1 > 65535 || d1 < 1){
		    				alert("Please set port between 1~65535");field.focus();return false;
		    			}
		    			if(d2 > 65535 || d2 < 1){
		    				alert("Please set port between 1~65535");field.focus();return false;
		    			}
		    			if(d1 >= d2){
		    				alert("Invaild setting");field.focus();return false;
		    			}
		    		}
		    	}
	   		}
		}
		
	}
	
	return true;
}

function checkInput(str)
{
	for (var i=0; i<str.length; i++) {
		if (str.charAt(i) == '\r' || str.charAt(i) == '\n' || str.charAt(i) == '^' || str.charAt(i) == '|'|| str.charAt(i) == '$' || str.charAt(i) == '`' || (str.charAt(i) == '%' ) ||str.charAt(i) == '&'|| str.charAt(i) == ';'|| (str.charAt(i) == "'" )  || (str.charAt(i) == '"' )  || (str.charAt(i) == '<' )  || (str.charAt(i) == '>' ))
		return 1;
	}
	return 0;
}

function formCheck(){
	//check rule num
    var nat_flag= "<% getCfg_OneToOneNat(1, "OneToOne_flag"); %>";
	if(nat_flag == "0"){
		if (!checkTableNum())
			return false;
    }

	//check name
    if(checkInput(document.NAT_APP.PortRule_Name.value)==1){
		alert('System do not support specific invalid characters.');
		return false;
    }
	  
	//check Port value
	if (!checkPortRule(document.NAT_APP.PortRule_Port))
		    return false;
/*
	if(nat_flag == "0"){
		if(checkDupliPort()==false){
			alert('Duplicate port not allowed to be set!');
			return false;
		}
	}*/
    
	natClick();
	
	//check Internal IP
	if (!checkIpAddr(document.NAT_APP.PortRule_IntAddr, false))
		    return false;
	if(document.NAT_APP.PortRule_IntAddr.value == lanIP ){
		alert('Internal ip could not be lan ip');
		return false;
	}
	if ( !checkSubnet(document.NAT_APP.lanIP.value, document.NAT_APP.lanMask.value, document.NAT_APP.PortRule_IntAddr.value)) {
        		alert('Invalid server ip address!It should be located in the same subnet of current LAN IP address.');
        		document.NAT_APP.PortRule_IntAddr.value = document.NAT_APP.PortRule_IntAddr.defaultValue;
        		document.NAT_APP.PortRule_IntAddr.focus();
        		return false;
    }
	//check external mapping IP by wind 2013-12-09
	if (!checkIpAddr(document.NAT_APP.PortRule_ExtAddr, false))
		    return false;
	if(document.NAT_APP.PortRule_ExtAddr.value == wanIP ){
		alert('External mapping ip could not be wan ip');
		return false;
	}
	if(nat_flag == "0"){
		if(checkDuplirule()==false){
			alert('Duplicate rule not allowed to be set!');
			return false;
		}
	}
	else if(nat_flag == "1"){
		if(editCheckDuplirule()==false){
			alert('Duplicate rule not allowed to be set!');
			return false;
		}
	}
	document.NAT_APP.PortFor_rtnFlag.value = "0";
	
	showWebMessage(2, "");
	return true;
}
function doCancel(){
		document.NAT_APP.PortFor_rtnFlag.value = "3";
		document.NAT_APP.submit();
}
function doEditRule(){
		document.NAT_APP.PortFor_rtnFlag.value = "1";
}
function doDeleteRule(){
		document.NAT_APP.PortFor_rtnFlag.value = "2";
		showWebMessage(2, "");
}
function doFindCurrentRule(CurrentIndex){
		
		document.NAT_APP.PortFor_rtnCurIndex.value = CurrentIndex;
}
function updateState(){
	initTranslation();
	
	if (document.NAT_APP.PortRule_Active.value == "1")
		document.NAT_APP.PortRule_Active.checked = true;
}
function checkValue(cstr){

}
</script>
</head>
<body onload="updateState()">
<form method="post" name="NAT_APP" action="/goform/oneToOne_natAPP" >
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="OneToOneRuleAdd">One-To-One NAT Rule</td>
                   </tr></table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="3%"><input name="PortRule_Active" type="checkbox" value="<% getCfg_OneToOneNat(1, "OneToOne_active"); %>" ></td>
<td width="97%" id="OneToOneActive">Active</td>                      
<td>&nbsp;</td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="OneToOneServiceName">Service Name : </td>
<td><input name="PortRule_Name" size="31" maxlength="31" value="<% getCfg_OneToOneNat(1, "OneToOne_service"); %>" type="text" />
<select name="Service_Name" size="1" onchange="doServiceChange(this.selectedIndex);">
<option value="00000000" selected="selected" id=User_Defined>User Defined </option>
<option value="00000001" id=Services>---Services--- </option>
<option value="00000002">DNS </option>
<option value="00000003">FTP </option>
<option value="00000004">HTTP </option>
<option value="00000005">HTTPS </option>
<option value="00000006">POP3 </option>
<option value="00000007">SMTP </option>
<option value="00000008">TELNET </option>
</select></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="InternalIP">Internal IP Address : </td>
<td><input name="PortRule_IntAddr" size="15" maxlength="15" value="<% getCfg_OneToOneNat(1, "int_ip"); %>"  type="text" /></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="ExternalIP">External Mapping IP Address : </td>
<td><input name="PortRule_ExtAddr" size="15" maxlength="15" value="<% getCfg_OneToOneNat(1, "ext_ip"); %>"  type="text" /></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="OneToOnePort">Forward Port : </td>
<td><input name="PortRule_Port" size="31" maxlength="99" value="<% getCfg_OneToOneNat(1, "forward_port"); %>" onkeypress="" type="text" /> (Ex: 10-20,30,40)
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="ServerForward" style="white-space:nowrap" >Server Forwarding</td>
<td width="3%" >
<input type="radio" value="1" name="forward_enable" <% getCfg_OneToOneNat(1, "forward_check1"); %>>
</td>
<td width="5%" >
<font id="ForwardEnable">Enable</font>
</td>
<td width="3%">
<input type="radio" value="0" name="forward_enable" <% getCfg_OneToOneNat(1, "forward_check2"); %>>
</td>
<td width="5%">
<font id="ForwardDisable">Disable</font>
</td>
<td width="77%"></td>
</tr>
</table>
</li>
<!--
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="applicationServerIP">Server IP Address : </td>
<td><input name="PortRule_SrvAddr" size="15" maxlength="15" value="<% getCfgNat(1, "nat_ip"); %>"  type="text" /></td>
</tr>
</table>
</li>
-->
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note" id ="oneNote">Note:</span></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note_a" id ="AlertNote">The max number of rules is 10.</span></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note_a" id ="one2oneNote">When NAT mode disable, the One-To-One rules setting is invalid.</span></td>
</tr>
</table>
</li>
<li class="space3"></li>
<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="OneToOneAppRuleSum">One To One NAT Rules Summary</td>
                   </tr></table>
</li>
<li class="space"></li>
<li class="w_text2">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><table width="100%" cellpadding="0" cellspacing="0" >
<tr>
<td height="26" colspan="8"><table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
<td background="images/table_top_center.gif"class="table_title" id="OneToOneAppRuleSum2">One To One NAT Rules Summary<span class="cTitle"></span></td>
<td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
</tr>
</table></td>
</tr>                
<tr>
<td width="5%"   align=center valign="top"><span class="top_left">#</span></td>
<td width="5%"   align=center><font class="top_font"><span id="OneToOneTdActive">Active</span></font></td>
<td width="15%"  align=center><font class="top_font"><span id="OneToOneTdName">Name</span></font></td>
<td width="15%"  align=center><font class="top_font"><span id="OneToOneTdIntIP">Internal IP</span></font></td>
<td width="15%"  align=center><font class="top_font"><span id="OneToOneTdExtIP">External IP</span></font></td>
<td width="15%"  align=center><font class="top_font"><span id="OneToOneTdPort">Port</span></font></td>
<td width="15%"  align=center><font class="top_font"><span id="OneToOneTdServerForward">Server Forwarding</span></font></td>          
<td width="15%"  align=center><font class="top_right"><span id="OneToOneTdModify">Modify</span></font></td>
</tr>
<% showOneToOneRulesASP(); %>
<tr>
<td height="5" colspan="8"><table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="5" height="5"><img src="images/table_bottom_left.gif" width="5" height="5" /></td>
<td height="5" background="images/table_bottom_center.gif"><img src="images/table_bottom_center.gif" width="1" height="5" /></td>
<td width="5"><img src="images/table_bottom_right.gif" width="5" height="5" /></td>
</tr>
</table></td>
</tr>
</table></td>
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
<input type="submit" value="Apply" id="natApply" name="addNAT_APP" onClick="return formCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="reset" value="Reset" id="natReset" name="reset" onClick="return doCancel()">
<input type=hidden value="/local/advance/one_to_one.asp" name="nat_app_url">
<input type="hidden" name="PortFor_rtnFlag" value="0" />
<input type="hidden" name="PortFor_rtnCurIndex" value="0" />
<input type=hidden name="lanIP" value="<% getCfgGeneral(1, "lan_ipaddr"); %>">
<input type=hidden name="lanMask" value="<% getCfgGeneral(1, "lan_netmask"); %>">
<input type=hidden name="wanIP" value="<% getCfgGeneral(1, "wan_ipaddr"); %>">
<input type=hidden name="wanMask" value="<% getCfgGeneral(1, "wan_netmask"); %>">
</li>
</center>             
</body>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script> 
</html>
