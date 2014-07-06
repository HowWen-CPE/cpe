<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="style/style.css" rel="stylesheet" type="text/css" />
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>td {white-space: nowrap;}</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="images/common.js"></script>
<script type="text/javascript" src="images/code.js"></script>
<script type="text/javascript" src="images/svg-common.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");
var F;
//var nv_OP_Mode = <% getCfgZero(1, "OP_Mode"); %>;

function isAllNum(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') )
			continue;
		return 0;
	}
	return 1;
}
function isIPformat(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
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

function checkIpAddr(field)
{
	if (isIPformat(field.value) == 0) {
		alert('It should be a [0-9] number.');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
	if ((!checkRange(field.value, 1, 0, 239)) ||
			(!checkRange(field.value, 2, 0, 255)) ||
			(!checkRange(field.value, 3, 0, 255)) ||
			(!checkRange(field.value, 4, 1, 254)))
	{
		alert('IP adress format error.');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
	return true;
}

function to_adtool(F, cmd) {
    if(cmd == "traceroute"){
         if (isAllNum(E(cmd+'cnt').value) == 0) {
     		alert('It should be a [1-15] number.');
     		E(cmd+'cnt').value = E(cmd+'cnt').defaultValue;
     		E(cmd+'cnt').focus();
     		return false;
         }
        if(E(cmd+'cnt').value == "" || E(cmd+'cnt').value < 1 || E(cmd+'cnt').value > 15)
        {
            alert('It should be a [1-15] number.');
            return false;
        }
    }else {
	     /*if(cmd == "ping"){
		   if(checkIpAddr(E(cmd+'host'))==false)
		     return false;
		 }*/
         if (isAllNum(E(cmd+'cnt').value) == 0) {
     		alert('It should be a [1-10] number.');
     		E(cmd+'cnt').select();
     		return false;
         }
        if(E(cmd+'cnt').value == "" || E(cmd+'cnt').value < 1 || E(cmd+'cnt').value > 10)
        {
     		alert('It should be a [1-10] number.');
     		E(cmd+'cnt').select();
            return false;
        }
    }
    F.action.value=cmd;
    F.submit_button.value = "ad_tools";
    ajax_preaction(F, "/goform/ad_tools");
    display_mask(F);
    return true;
}

function to_adtool_arp(F, cmd) {
    if (E(cmd+'host').value == "") {
        alert("Needs a domain name or an IP address");
        return false;
    }
    checkIpAddr(E(cmd+'host')); 
    if(cmd == "traceroute"){
         if (isAllNum(E(cmd+'cnt').value) == 0) {
     		alert('It should be a [1-15] number.');
     		E(cmd+'cnt').value = E(cmd+'cnt').defaultValue;
     		E(cmd+'cnt').focus();
     		return false;
         }
        if(E(cmd+'cnt').value == "" || E(cmd+'cnt').value < 1 || E(cmd+'cnt').value > 15)
        {
            alert("Needs a specific trace route counts!");
            return false;
        }
    }else {
         if (isAllNum(E(cmd+'cnt').value) == 0) {
     		alert('It should be a [1-10] number.');
     		E(cmd+'cnt').value = E(cmd+'cnt').defaultValue;
     		E(cmd+'cnt').focus();
     		return false;
         }
        if(E(cmd+'cnt').value == "" || E(cmd+'cnt').value < 1 || E(cmd+'cnt').value > 10)
        {
            alert("Needs a specific packets num!");
            return false;
        }
    }
    F.action.value=cmd;
    F.submit_button.value = "ad_tools";
    ajax_preaction(F, "/goform/ad_tools");
    display_mask(F);
    return true;
}
function init() {

  	e = document.getElementById("ping_host");
	e.innerHTML = _("ping host");
	
	e = document.getElementById("ping_package");
	e.innerHTML = _("ping package");
	
	//e = document.getElementById("arpping_host");
	//e.innerHTML = _("arpping host");
	
	//e = document.getElementById("arpping_package");
	//e.innerHTML = _("arpping package");
		
	e = document.getElementById("traceroute_host");
	e.innerHTML = _("traceroute host");
	
	e = document.getElementById("traceroute_hop");
	e.innerHTML = _("traceroute hop");
	
  F = document.getElementsByName("form")[0];
}
</script>
</head>
<body onload="init()">
<form method="post" name="ad_tools" action="apply.cgi" >
  <script type="text/javascript">do_hidden_elements('adtool_rule')</script>
  <div id="table">
    <ul>
      <li class="table_content">
        <div class="data">
          <ul>
            <li class="title" id="remoteManagementWWW"><% lang("Ping"); %></li>
              <li class="w_text" style="visibility:hidden;display:none">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="40%" id=""><% lang("Interface"); %> : </td>
                    <td><select name="pingiface" id="pingiface" size="1" >
                    <option value="any" <%nvg_attr_match("pingiface","adtool_rule", 0, "pingiface", "any", "selected");%>><% lang("*"); %></option>
                    <% ad_tools_showif("wan","ping"); %>
                  	<% ad_tools_showif("lan","ping"); %>
                    </select> 
                    </td>
                  </tr>
                </table>
              </li> 
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="40%" id="ping_host"> <% lang("Target Host"); %> : </td>
                    <td><input name="pinghost" id="pinghost" size="32" maxlength="32" value="<% nvg_attr_get("pinghost", "adtool_rule", "0", "pinghost"); %>" type="text" /></td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                <tr>
                  <td width="40%" id="ping_package"><% lang("Number of Packets"); %> : </td>
                  <td><input name="pingcnt" id="pingcnt" size="32" maxlength="2" value="<% nvg_attr_get("pingcnt", "adtool_rule", "0", "pingcnt"); %>" type="text" /> Packets (1 ~ 10)</td>
                </tr>
              </table>
            </li>
            <li class="w_text">
              <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                <tr>
                  <td width="40%" id=""> <% lang("Ping"); %> : </td>
                  <td><input id="pingtest" value="Ping" name="pingtest" onclick="to_adtool(this.form, 'ping')" type="button"> &nbsp;&nbsp;</td>
                </tr>
              </table>
            </li>
<!--
          <li class="space3"></li>
          <li class="title" id="ARPing"><% lang("ARPing"); %> (<% lang("Within the same broadcasting domain"); %>)</li>   
-->
<script>
/*
if(nv_OP_Mode == 1 || nv_OP_Mode == 5)
	document.write("<li class='w_text' style='visibility:hidden;display:none'>");
else
	document.write("<li class='w_text'>");
*/
</script>
<!--
            <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
              <tr>
                <td width="40%" id=""><% lang("Interface"); %> : </td>
                <td><select name="arpingiface" id="arpingiface" size="1" >
                    <% ad_tools_showif("lan","arping"); %>
                  	<% ad_tools_showif("wan","arping"); %>
                  </select>
                </td>
              </tr>
          </table>
        </li>
        <li class="w_text">
          <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
              <td width="40%" id="arpping_host"> <% lang("Target Host"); %> : </td>
              <td><input name="arpinghost" id="arpinghost" size="32" maxlength="32" value="<% nvg_attr_get("arpinghost", "adtool_rule", "0", "arpinghost"); %>" type="text" /></td>
            </tr>
          </table>
        </li>
        <li class="w_text">
          <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
              <td width="40%" id="arpping_package"> <% lang("Number of Packets"); %> : </td>
              <td><input name="arpingcnt" id="arpingcnt" size="32" maxlength="2" value="<% nvg_attr_get("arpingcnt", "adtool_rule", "0", "arpingcnt"); %>" type="text" /> <%lang("Packets"); %> <% lang("(1 ~ 10)"); %> </td>
            </tr>
          </table>
        </li>
        <li class="w_text">
          <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
              <td width="40%" id=""> <% lang("ARPing"); %> : </td>
              <td><input id="arpingtest" value="ARPing" name="arpingtest" onclick="to_adtool_arp(this.form, 'arping')" type="button"> &nbsp;&nbsp;</td>
            </tr>
          </table>
        </li>
-->
        <li class="space3"></li>
        <li class="title" id="TraceRoute"><% lang("Trace Route"); %></li>
        <li class="w_text" style="visibility:hidden;display:none">
          <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
              <td width="40%" id="">Interface : </td>
              <td>
                <select name="tracerouteiface" size="1" id="tracerouteiface">
                <option value="any" <%nvg_attr_match("tracerouteiface","adtool_rule", 0, "tracerouteiface", "any", "selected");%>><% lang("*"); %></option>
                <% ad_tools_showif("wan","traceroute"); %>
                </select>
              </td>
            </tr>
          </table>
        </li>
        <li class="w_text">
          <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
              <td width="40%" id="traceroute_host"> <% lang("Target Host"); %> : </td>
              <td><input name="traceroutehost" id="traceroutehost" size="32" maxlength="32" value="<% nvg_attr_get("traceroutehost", "adtool_rule", "0", "traceroutehost"); %>" type="text" /></td>
            </tr>
          </table>
        </li>
        <li class="w_text">
          <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
              <td width="40%" id="traceroute_hop"> <% lang("Hop Count"); %> : </td>
              <td><input name="traceroutecnt" id="traceroutecnt" size="32" maxlength="2" value="<% nvg_attr_get("traceroutecnt", "adtool_rule", "0", "traceroutecnt"); %>" type="text" /><%lang("Counts"); %> <% lang("(1 ~ 15)"); %></td>
            </tr>
          </table>
        </li>
        <li class="w_text">
          <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
              <td width="40%" id=""> <% lang("Trace route"); %> : </td>
              <td><input id="adTraceRoute" value="Trace Route" name="adTraceRoute" onclick="to_adtool(this.form, 'traceroute')" type="button"> &nbsp;&nbsp;</td>
            </tr>
          </table>
        </li>
        <li class="w_text">
          <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
              <td><textarea rows="6" readonly="readonly" ><% st_show_traceroute();%></textarea></td>
            </tr>
        </table>
      </li>

      <li></li>
      <li></li>
      <li></li>
      <li></li>
      <li></li>
      <li></li>
      <li></li>
      <li></li>
      <li></li>
      <li></li>
      </ul>
      </div>
    </li>
    <center>
      <li class="table_button">
      <input type=hidden value="/local/advance/ad_tools.asp" name="ad_tools_url">
      </li>
    </center>
  </form>
    <script type="text/javascript">create_background_cover()</script>
    <script type="text/javascript">create_waiting_window()</script>
    <script type="text/javascript">create_return_window()</script>
</body>
<script>
function alert(str)
{
  showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script> 
</html>
