<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">

<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>

<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
 
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="images/common.js"></script>
<script type="text/javascript" src="images/code.js"></script>
<script type="text/javascript" src="images/svg-common.js"></script>

<script language="JavaScript" type="text/javascript">

Butterlate.setTextDomain("internet");

var l2tpOIPsec_status = '<% getCfgZero(1, "l2tpOIPsec_status"); %>';
var l2tpServerEnable = '<% getCfgZero(1, "l2tpServer_status"); %>';

function initTranslation(){
	var e = document.getElementById("ipsecEnable");
	e.innerHTML = _("stp set 1");

	e = document.getElementById("ipsecDisable");
	e.innerHTML = _("stp set 0");
	
	e = document.getElementById("ipsecApply");
	e.value = _("stp Apply");
	
	e = document.getElementById("ipsecReset");
	e.value = _("stp Reset");

} 

function formCheck()
{
     if(l2tpServerEnable == 1){
	  if (l2tpOIPsec_status == 1){
		alert('Please disable l2tpOIPsec function on l2tp server setting.');
		return false;
	  }
     }
	showWebMessage(2, "");
	return true;
}

function init() {
     initTranslation();
     enable="<%nvg_match("ipsec_enable","1","checked");%>";
     if(enable == "checked")
         document.vpn_ipsec.ipsec_enable[0].checked = true;
     disable="<%nvg_match("ipsec_enable","0","checked");%>";
     if(disable == "checked")
         document.vpn_ipsec.ipsec_enable[1].checked = true;
}
</script>
<body onload="init();">
  <form name="vpn_ipsec" method="post" action="/goform/vpn_ipsec_general"  onSubmit="return formCheck()">
    <div id="table">
      <ul>
        <li class="table_content">
          <div class="data">
            <ul>
              <li class="title" id="securityTitle">
                IPsec
              </li>
              <span class='on'>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
				  <input type="radio" value="1" name="ipsec_enable" ><font id="ipsecEnable">Enable</font>
				  <input type="radio" value="0" name="ipsec_enable" ><font id="ipsecDisable">Disable</font>
		   </table>
		 </li>
		</span>
                <li></li>
                <li></li>

            </ul>
          </div>
          </li>
    <center>
      <li class="table_button">
	    <input type="submit" value="Apply" id="ipsecApply" name="add" onClick="return formCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type="reset" value="Cancel" id="ipsecReset" name="reset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/vpn_ipsec_general.asp" name="ipsec_gen_url">
      </li>
    </center>
      </ul>
    </div>
  </form>
</body>
<script>
  function alert(str) {
    showWebMessage(1, str);
  }
  showWebMessage( <%getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script>

</html>
