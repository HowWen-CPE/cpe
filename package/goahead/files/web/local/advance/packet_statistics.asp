<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Expires" content="-1">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv="Pragma" content="no-cache">
<script type="text/javascript" src="/lang/b28n.js"></script>
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />

<style>
.on {display:on}
.off {display:none}
td {white-space: nowrap;}
</style>

<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("wireless");

var CallPacketStatistic = "<% GetPortIfBytesPerSecASP(); %>";
var nv_OP_Mode = <% getCfgZero(1, "OP_Mode"); %>;

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

function initTranslation()
{	
	var e = document.getElementById("StatisticsTitle");
	e.innerHTML = _("packet statis prtitle");

	e = document.getElementById("StatisticsTableTitle");
	e.innerHTML = _("packet statis tabletitle");

	var e = document.getElementById("StatisticsPort");
	e.innerHTML = _("packet statis port");

	e = document.getElementById("StatisticsStatus");
	e.innerHTML = _("packet statis status");
	
	var e = document.getElementById("StatisticsTxPkts");
	e.innerHTML = _("packet statis txpkts");

	e = document.getElementById("StatisticsRxPkts");
	e.innerHTML = _("packet statis rxpkts");
	
	var e = document.getElementById("StatisticsCollisions");
	e.innerHTML = _("packet statis collis");

	e = document.getElementById("StatisticsTXBS");
	e.innerHTML = _("packet statis txbs");
	
	var e = document.getElementById("StatisticsRXBS");
	e.innerHTML = _("packet statis rxbs");

	//e = document.getElementById("StatisticsUpTime");
	//e.innerHTML = _("packet statis uptime");

	e = document.getElementById("StatisticsSysTime");
	e.innerHTML = _("packet statis systime");
	
	e = document.getElementById("StatisticsSysTime");
	e.innerHTML = _("packet statis systime");
	
	e = document.getElementById("StatisticsPollInterval");
	e.innerHTML = _("packet statis opllinterval");
	
//	e = document.getElementById("StatisticsTimeSec");
//	e.innerHTML = _("packet statis timesec");

	e = document.getElementById("StatisticsSetInterval");
	e.value = _("packet statis set interval");
	
	e = document.getElementById("StatisticsStop");
	e.value = _("packet statis set stop");

	e = document.getElementById("dashRefNone");
	e.innerHTML = _("dash ref none");
	e = document.getElementById("dashRef5s");
	e.innerHTML = _("dash ref 5s");
	e = document.getElementById("dashRef10s");
	e.innerHTML = _("dash ref 10s");
	e = document.getElementById("dashRef15s");
	e.innerHTML = _("dash ref 15s");
	e = document.getElementById("dashRef20s");
	e.innerHTML = _("dash ref 20s");
	e = document.getElementById("dashRef25s");
	e.innerHTML = _("dash ref 25s");
}

function PageInit()
{
	initTranslation();
}

function PortStatistics()
{
	var i=0;
	var portSt = "<% getPortsStatistics(); %>";
	var portStary = new Array;
	portStary = portSt.split("$");
	for (i=0; i<portStary.length; i++) {
		var temp = new Array;
		temp = portStary[i].split(";");
		document.write("<tr ><td width='10%'><center>");
		if (i == 0) {
			document.write("<span class='table_left'><font id ='StatisticsPort1'>Port1 </font>&nbsp;</span>");
			var e = document.getElementById("StatisticsPort1");
			e.innerHTML = _("packet statis port1");
		} else if (i == 1) {
			document.write("<span class='table_left'><font id ='StatisticsPort2'>Port2 </font>&nbsp;</span>");
			var e = document.getElementById("StatisticsPort2");
			e.innerHTML = _("packet statis port2");
		} else if (i == 2) {
			document.write("<span class='table_left'><font id ='StatisticsPort3'>Port3 </font>&nbsp;</span>");
			var e = document.getElementById("StatisticsPort3");
			e.innerHTML = _("packet statis port3");
		} else if (i == 3) {
			document.write("<span class='table_left'><font id ='StatisticsPort4'>Port4 </font>&nbsp;</span>");
			var e = document.getElementById("StatisticsPort4");
			e.innerHTML = _("packet statis port4");
		} else if (i == 4) {
			document.write("<span class='table_left'><font id ='StatisticsPort5'>Port5 </font>&nbsp;</span>");
			var e = document.getElementById("StatisticsPort5");
			e.innerHTML = _("packet statis port5");
		}
		document.write("</center></td>");
		document.write("<td width='10%'><center>");
		document.write("<span class='table_font'>");
		document.write(temp[0]);
		document.write("</span></center></td>");
		document.write("<td width='17%'><center>");
		document.write("<span class='table_font'>");
		document.write(temp[1]);
		document.write("</span></center></td>");
		document.write("<td width='17%'><center>");
		document.write("<span class='table_font'>");
		document.write(temp[2]);
		document.write("</span></center></td>");
		document.write("<td width='12%'><center>");
		document.write("<span class='table_font'>");
		document.write(temp[3]);
		document.write("</span></center></td>");
		document.write("<td width='17%'><center>");
		document.write("<span class='table_font'>");
		document.write(temp[4]);
		document.write("</span></center></td>");
		document.write("<td width='17%'><center>");
		document.write("<span class='table_right'>");
		document.write(temp[5]);
		document.write("</span></center></td>");
		document.write("</tr>");
	}
}

</script>
</head>
<body onLoad="PageInit()">
<form method=post name="SetIntervalForm" action="/goform/SetIntervalForm">
<input type=hidden value="/local/advance/packet_statistics.asp" name="url">

<!-- Tab --> 
<div id="table">
  <ul>
    <li class="table_content">
      <div class="data">
          <ul>
				<li class="title" id="StatisticsTitle">Port Statistics</li>
              <li></li>
  <li class="w_text2">
          
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                          <tr>
                            <td height="26" colspan="8"><table width="100%" border="0" cellspacing="0" cellpadding="0">
                                <tr>
                                  <td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
                                  <td background="images/table_top_center.gif"class="table_title" id="StatisticsTableTitle">Packet Statistics </td>
                                  <td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
                                </tr>
                            </table></td>
                          </tr>
<!--                                  
                        </table></li>
  <li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
-->
                                  <tr >
                                    <td width="10%"><center>
                                        <span class="top_left"><font id ="StatisticsPort">Port</font></span>
                                                                                                                                                </center></td>
									
                                    <td width="10%"><center>
                                      <span class="top_font"><font id ="StatisticsStatus">Status</font></span>                                                                                                          </center></td>
									
                                    <td width="17%"><center>
                                        <span class="top_font"><font id ="StatisticsTxPkts">TxPkts </font></span>
                                                                                                                                                </center></td>
                                    <td width="17%"><center>
                                        <span class="top_font"><font id ="StatisticsRxPkts">RxPkts </font></span>
                                                                                                            </center></td>
                                    <td width="12%"><center>
                                        <span class="top_font"><font id ="StatisticsCollisions">Collisions </font></span>
                                                                                                            </center></td>
                                    <td width="17%"><center>
                                        <span class="top_font"><font id ="StatisticsTXBS">Tx B/s </font></span>
                                                                                                                                                </center></td>
                                    <td width="17%"><center>
                                        <span class="top_right"><font id ="StatisticsRXBS">Rx B/s </font></span>
                                                                                                            </center></td>
<!--
                                    <td width="20%"><center>
                                        <span class="top_right"><font id ="StatisticsUpTime">Up Time </font></span>
                                                                                                                                                                                    </center></td>
-->
                                  </tr>
<!--                                  
                        </table></li>
-->                        
<script>
	PortStatistics();
</script>

                          <tr>
                            <td height="5" colspan="8"><table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
                                <tr>
                                  <td width="5" height="5"><img src="images/table_bottom_left.gif" width="5" height="5" /></td>
                                  <td height="5" background="images/table_bottom_center.gif"><img src="images/table_bottom_center.gif" width="1" height="5" /></td>
                                  <td width="5"><img src="images/table_bottom_right.gif" width="5" height="5" /></td>
                                </tr>
                            </table></td>
                          </tr>
                        </table></li>
          </ul>
          
          <ul>
            <li class="w_text">
              <ul>
                <li class="all_table">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap>
                <a id="StatisticsSysTime">System Up Time :</a>&nbsp;<% getSysUptime(); %>
                   </td>
                   </tr>
                  </table>
                  </li>
              </ul>
            </li>
          </ul>
          <ul>
            <li class="w_text">
              <ul>
                <li class="all_table">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap>
                <b><font id ="StatisticsPollInterval">Poll Interval(s) :</font></b>
                    <select name="interval" size="1">
                      <option value="00000000" selected="selected" id="dashRefNone"> </option>
                      <option value="00000001" id="dashRef5s" ></option>
                      <option value="00000002" id="dashRef10s"></option>
                      <option value="00000003" id="dashRef15s"></option>
                      <option value="00000004" id="dashRef20s"></option>
                      <option value="00000005" id="dashRef25s">300 seconds</option>
                    </select>
                  &nbsp;&nbsp;                               
<!--                  
                  <font color="#ffffff" id="StatisticsTimeSec">sec </font>&nbsp;&nbsp;
-->                  
                  <input onclick="parent.polling_interval=document.SetIntervalForm.interval.selectedIndex*60;document.SetIntervalForm.submit();" type="button" value="Set Interval" id="StatisticsSetInterval" name="SetIntvl" &Acirc;?80? />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input onclick="clearTimeout(time_id); parent.polling_interval=0; document.SetIntervalForm.submit();" type="button" value="Stop" id="StatisticsStop" name="stopIntvl" &Acirc;?80? />
                  </table>
                </li>
              </ul>
            </li>
          </ul>
          <ul>
            <li></li>
          </ul>
          <ul>
            <li></li>
          </ul>
          </div>
    </li>
    <li class="table_button"></li>
  </ul>
</div>
</form>
</body>
<script>
document.SetIntervalForm.interval.selectedIndex=parent.polling_interval/60;
var	time_id=0;
if (parent.polling_interval>0)
	time_id=self.setTimeout('self.location.reload();', parent.polling_interval*1000);
</script>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script> 
</html>
