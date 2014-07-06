<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>td {white-space: nowrap;}</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">

Butterlate.setTextDomain("internet");

function initTranslation(){
	var e = document.getElementById("RoutingTable");
	e.innerHTML = _("route_table");	

	e = document.getElementById("display_info");
	e.innerHTML = _("routing del title");

	e = document.getElementById("showRouting");
	e.value= _("dhcpClient Refresh");
}

function CheckValue()
{   
    showWebMessage(2, "");
    return true;
}


function init() {
       initTranslation();
}

</script>
</head>
<body onload="init()">
<form method="post" name="route_table" action="/goform/show_routing" onSubmit="return CheckValue()">
  <div id="table">
    <ul>
      <li class="table_content">
        <div class="data">
        <ul>         
        <li class="title" id="RoutingTable">Routing Table</li>
        <li class="w_text">
          <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
              <td width="40%" id="display_info">Current Routing table in the system:</td>
              <td><input type=reset id="showRouting" value="Refresh" name="showRouting" onclick="window.location.reload()" > &nbsp;&nbsp;</td>
            </tr>
          </table>
        </li>
        <li class="w_text">
          <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
              <td><textarea rows="6" wrap="off" cols="100"  font-family:monospace readonly="readonly" ><% st_show_routing();%></textarea></td>
            </tr>
        </table>
      </li>
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
      <input type=hidden value="/local/advance/route_table.asp" name="route_table_url">
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
