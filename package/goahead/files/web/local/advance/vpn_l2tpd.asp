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
<script type="text/javascript" src="/local/advance/table.js"></script>
<script type="text/javascript" src="images/code.js"></script>
<script type="text/javascript" src="images/svg-common.js"></script>
<script language="JavaScript" type="text/javascript">

Butterlate.setTextDomain("firewall");

var doneFlag = 0;
var rule;
var edit_idx;
var F; 
var flag = 0;

var default_rule = ['','',''];

function initTranslation(){
	var e = document.getElementById("addRules");
	e.innerHTML = _("add Rules");
	e = document.getElementById("ruleEnable");
	e.innerHTML = _("rule Enable");
	e = document.getElementById("l2tpdUserName");
	e.innerHTML = _("pptpd User Name");
	e = document.getElementById("l2tpdPassword");
	e.innerHTML = _("pptpd Password");
	e = document.getElementById("l2tpd_table_add");
	e.value = _("table Add");
	
	e = document.getElementById("userRuleSum");
	e.innerHTML = _("user RuleSum");
	e = document.getElementById("userRuleSum2");
	e.innerHTML = _("user RuleSum");
	e = document.getElementById("ruleEnable2");
	e.innerHTML = _("rule Enable");
	e = document.getElementById("l2tpdUserName2");
	e.innerHTML = _("pptpd User Name");
	e = document.getElementById("l2tpdPassword2");
	e.innerHTML = _("pptpd Password");
	e = document.getElementById("l2tpdModify");
	e.innerHTML = _("pptpd Modify");
	e = document.getElementById("VPNReset");
	e.value = _("VPN Reset");
			
} 

//don't include characters "^", "|", """
function isUserName(s)
{
	for(var i=0; i<s.length; i++) {
		if( s.charAt(i) == '^' || s.charAt(i) == '"' ||
		    s.charAt(i) == "|" )
		    return true;
	}
	return false;	
}

//don't include characters "^", "|", """
function isPassword(s)
{
	for(var i=0; i<s.length; i++) {
		if( s.charAt(i) == '^' || s.charAt(i) == '"' ||
		    s.charAt(i) == "|" )
		    return true;
	}
	return false;	
}

function local_rule_check() {

    if ((!E('user_name_l2tpd').value)) {
        alert('User name is required.');
        return false;
    }

    if (isUserName(E('user_name_l2tpd').value) == true) {
        alert('User name error. Chatacter Caret,Pile Sign and Double Quote are not supported in this field.');
        return false;
    }

    if (!E('user_password_l2tpd').value) {
        alert('Password is required.');
        return false;
    }

    if (isPassword(E('user_password_l2tpd').value) == true) {
        alert('Password error. Chatacter Caret,Pile Sign and Double Quote are not supported in this field.');
	return false;
    }

    return true;
}
function check_box_set(id, value) {
	if (value == '1') {
		E(id).checked = true;
	}
	else
	{
		E(id).checked = false;
	}
}
var enum_l2tp = {                                                                                                          
    _ENABLE:0,                                                                                                             
    _NAME:1,                                                                                                               
    _PROVIDER:2,                                                                                                           
    _PASSWD:3,                                                                                                             
    _IPADDR:4                                                                                                              
} 

function local_fill_input_field() {
	check_box_set('user_rule_enable_l2tpd', rule[enum_l2tp._ENABLE]);
    E('user_name_l2tpd').value = rule[enum_l2tp._NAME];
    E('user_password_l2tpd').value = rule[enum_l2tp._PASSWD];
}

function local_create_table(T, R) {
    var c;

    c =	"<li class=\"w_text2\">"
		+ "<table width=\"95%\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\">"
		+ "<tr>"
		+ "<td>"
		+ "<table width=\"100%\" cellpadding=\"0\" cellspacing=\"0\" >"
		+ "<tr>"
		+ "<td height=\"26\" colspan=\"5\">"
		+ "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">"
		+ "<tr>"
		+ "<td width=\"5\" height=\"26\">"
		+ "<img src=\"images/table_top_left.gif\" width=\"5\" height=\"26\" />"
		+ "</td>"
		+ "<td background=\"images/table_top_center.gif\"class=\"table_title\" id=\"userRuleSum2\">"
		+ "User Rules Summary"
		+ "<span class=\"cTitle\">"
		+ "</span>"
		+ "</td>"
		+ "<td width=\"5\" height=\"26\">"
		+ "<img src=\"images/table_top_right.gif\" width=\"5\" height=\"26\" />"
		+ "</td>"
		+ "</tr>"
		+ "</table>" + "</td>"
		+ "</tr>"                
		+ "<tr>"
		+ "<td width=\"5%\"   align=center valign=\"top\">"
		+ "<span class=\"top_left\">"
		+ "#"
		+ "</span>"+ "</td>"

		+ "<td width=\"10%\"   align=center>"  + "<font class=\"top_font\">"  + "<span id=\"ruleEnable2\">" 
		+ "Rule Enable" 
		+ "</span>" + "</font>"  + "</td>"
		+ "<td width=\"25%\"  align=center>"  + "<font class=\"top_font\">"  + "<span id=\"l2tpdUserName2\">"
		+ "User Name" 
		+ "</span>"  + "</font>" + "</td>" 
		+ "<td width=\"25%\"  align=center>"  + "<font class=\"top_font\">" + "<span id=\"l2tpdPassword2\">" 
		+ "Password" + "</span>"+ "</font>"+ "</td>"          
		+ "<td width=\"10%\"  align=center>"
		+ "<font class=\"top_right\">"
		+ "<span id=\"l2tpdModify\">"
		+ "Modify"
		+ "</span>"
		+ "</font>"
		+ "</td>"
		+ "</tr>";

    var cur_num = parseInt(E(T+"_cur_rule_num").value);
	var j;
    for (var i = 0; i < cur_num; i++) {
		j = i + 1;
        c += "<tr>"
    	       + "<td valign='top'><center><span class='table_left'>" + (i+1) + "</span></center></td>\n"
              + "<td width=\"10%\"   align=center><center><span class=\"table_font\">" + tbl_wrap_enable(R[i][enum_l2tp._ENABLE]) + "</span></center></td>\n"
              + "<td width=\"10%\"   align=center><center><span class=\"table_font\">" + R[i][enum_l2tp._NAME] + "</span></center></td>\n"
              + "<td width=\"10%\"   align=center><center><span class=\"table_font\">" + R[i][enum_l2tp._PASSWD] + "</span></center></td>\n"
    
    	       +"<td style=\"white-space:nowrap\"><center><span class=\"table_right\">\n"
      	       + "<a href=\"javascript: doEditRule("+i+");\" onmouseout=\"MM_swapImgRestore()\" onmouseover=\"MM_swapImage('fnatEImage" + j + "','','images/i_edit_on.gif',1)\">"
       	+"<img src=\"images/i_edit.gif\" name=\"fnatEImage\" width=\"18\" height=\"18\" border=\"0\" id=\"fnatEImage" + j + "\"  title=\"Edit\"/>"
       	+"<a href=\"javascript: to_submit('l2tpd_table', 'del', " + i + ",'','l2tpd_user_rule');\" onmouseout=\"MM_swapImgRestore()\" onmouseover=\"MM_swapImage('fnatDImage" + j + "','','images/i_delete_on.gif',1)\">"
       	+"<img src=\"images/i_delete.gif\" name=\"fnatDImage\" width=\"18\" height=\"18\" border=\"0\" id=\"fnatDImage" + j + "\"  title=\"Delete\"/>\n"
       	+"</a></span></center></td>\n"
       	+"</tr>\n"  
	}

	c += "<tr>"
		+ "<td height=\"5\" colspan=\"5\">"
		+ "<table width=\"100%\" height=\"5\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">"
		+ "<tr>"
		+ "<td width=\"5\" height=\"5\">"
		+ "<img src=\"images/table_bottom_left.gif\" width=\"5\" height=\"5\" />"
		+ "</td>"
		+ "<td height=\"5\" background=\"images/table_bottom_center.gif\">"
		+ "<img src=\"images/table_bottom_center.gif\" width=\"1\" height=\"5\" />"
		+ "</td>"

		+ "<td width=\"5\">"
		+ "<img src=\"images/table_bottom_right.gif\" width=\"5\" height=\"5\" />"
		+ "</td>"
		+ "</tr>"
		+ "</table>"
		+ "</td>"
		+ "</tr>"
		+ "</table>"
		+ "</td>"
		+ "</tr>"
		+ "</table>"
		+ "</li>";

    return c;
}

function local_pack_key_value_pair(F, T, prefix) {
    /* Important */
    tbl_append_element(F, 'l2tpd_rule', '');
    var row_num = parseInt(E(T+'_cur_rule_num').value);

    tbl_append_element(F, prefix + "_num", row_num);

    for (var i = 0; i < row_num; i++) {
        tbl_append_element(F, prefix + i + '_enable', l2tpd_user_rule[i][enum_l2tp._ENABLE]);
        tbl_append_element(F, prefix + i + '_username', l2tpd_user_rule[i][enum_l2tp._NAME]);
        tbl_append_element(F, prefix + i + '_provider', l2tpd_user_rule[i][enum_l2tp._PROVIDER]);
        tbl_append_element(F, prefix + i + '_passwd', l2tpd_user_rule[i][enum_l2tp._PASSWD]);
        tbl_append_element(F, prefix + i + '_ipaddr', l2tpd_user_rule[i][enum_l2tp._IPADDR]);
    }
}

function local_generate_rule(T) {

    var rule_tmp = Array();

    rule_tmp[enum_l2tp._ENABLE]   = tbl_dewrap_enable('user_rule_enable_l2tpd');
    rule_tmp[enum_l2tp._NAME]     = E('user_name_l2tpd').value;
    rule_tmp[enum_l2tp._PROVIDER] = '';
    rule_tmp[enum_l2tp._PASSWD]   = E('user_password_l2tpd').value;
    rule_tmp[enum_l2tp._IPADDR]   = '';

    return rule_tmp;
}

function tbl_del_tr_l2tpd(T, R, idx) {

	var i = parseInt(E(T+'_cur_rule_num').value) - 1;

	E(T+'_cur_rule_num').value = i;
	R.splice(idx, 1);
}

function rule_list_check_dup(T, R, index, row) {
    var row_num = row+1;
    for (var i = 0; i < row_num; i++) {

        var name1 = R[i][index];
        for (var j = i+1; j < row_num; j++) {

            var name2 = R[j][index];
            if (name1 == name2) {
				alert('User name repeats.');
				return false;
			}
        }
    }
    return true;
}

function local_table_check(T, R) {

	/* Check the rule num */
	if (parseInt(E(T+'_cur_rule_num').value) >= parseInt(E(T+"_max_rule_num").value)) {
		alert('The max number of the rule is 32!');
		return false;
	}

	var tmp = R.clone();
	var i = parseInt(E(T+'_cur_rule_num').value);
	var s = local_generate_rule(T);
    tmp.move(i, i, s);

	/* Check the duplicate Conn Name */
	if (rule_list_check_dup(T, tmp, enum_l2tp._NAME, i) == false) {
		return false;
	} 

	return true;
}

function rule_list_name_check(R, index) {

	var row_num = parseInt(E('l2tpd_table_cur_rule_num').value);

	var name1 = R[edit_idx][index];

	for (var j = 0; j < row_num; j++) {
		if (edit_idx == j)
			continue;
		var name2 = R[j][index];

		if (name1 == name2) {
			alert('User name repeats.');
			return false;
		}
	}
	return true;
}

function local_create_input_field(T, nth, r) {
	rule = (r != undefined) ? r : default_rule;
}

function doEditRule(idx) {
	local_create_input_field("l2tpd_table", -1, l2tpd_user_rule[idx]);
	local_fill_input_field(); 
	edit_idx = idx;
       flag = 1;
	   
	//E('l2tpd_table_add').style.display = "none";
	//E('l2tpd_table_edit').style.display = "block";
}

function edit_submit(edit_idx, R) {

	//avoid IE reentrant
	if(doneFlag != 0) {
		return;
	}
	doneFlag = 1;

	if (local_rule_check() == false) {
		return;
	}

	var s = local_generate_rule("l2tpd_table");
	//delete
	R.splice(edit_idx, 1);

	//add
	R.splice(edit_idx, 0, s);

	if (rule_list_name_check(l2tpd_user_rule, enum_l2tp._NAME ) == false) {
		return;
	}

    local_pack_key_value_pair(F, "l2tpd_table", "l2tpd_user_rule");

	showWebMessage(2, "");

	//clean
	edit_idx = '';

	F.submit();
}

function to_submit(T, act, idx, edit_idx, R) {

	//avoid IE reentrant
	if(act == "del") {
		if (doneFlag == 0) {

			doneFlag = 1;

			tbl_del_tr_l2tpd(T, l2tpd_user_rule, idx); 

			local_pack_key_value_pair(F, "l2tpd_table", "l2tpd_user_rule");

			showWebMessage(2, "");
			F.submit();
			return;
		} else {
			return;
		}
	}

	if (local_rule_check() == false) {
		return;
	}

	if (act == "add") {
		if (flag == 0){
			if (local_table_check(T, l2tpd_user_rule) == false) {
				return;
			}
			var s = local_generate_rule();
			tbl_insert_tr(T, l2tpd_user_rule, s, parseInt(E(T+'_cur_rule_num').value), parseInt(E(T+'_cur_rule_num').value));
			local_pack_key_value_pair(F, "l2tpd_table", "l2tpd_user_rule");
			showWebMessage(2, "");
    			F.submit();
		}else if (flag == 1){
			var tmp = R.clone();
			var s = local_generate_rule("l2tpd_table");
			tmp.move(edit_idx,edit_idx,s);

			if (rule_list_name_check(tmp, enum_l2tp._NAME ) == false) {
				return;
			}
			//delete
			R.splice(edit_idx, 1);

			//add
			R.splice(edit_idx, 0, s);
    			local_pack_key_value_pair(F, "l2tpd_table", "l2tpd_user_rule");
			flag = 0;
			showWebMessage(2, "");
			edit_idx = '';
			F.submit();		
		}
	}
}
function init() {
    initTranslation();
    F = document.getElementById("vpn_l2tpd_user_rule");
}
</script>
</head>
  <body onload="init();">

  <form id="vpn_l2tpd_user_rule" name="vpn_l2tpd_user_rule" action="/goform/vpn_l2tpd_user_rule" method="post">

<% tbl_get("l2tpd_user_rule"); %>
  <div id="table">
    <ul>
      <li class="table_content">
        <div class="data">
          <ul>
              <fieldset>

			  <li class="title" id="addRules">Add User Rule </li> 

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="40%" id="ruleEnable">Rule Enable </td>
						<td>
						<input id='user_rule_enable_l2tpd' name='user_rule_enable_l2tpd' type='checkbox'/>
						  </td>
						</tr>
					</table>
				</li>
				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>

						<td width="40%" id="l2tpdUserName">User Name </td>
						<td><input id="user_name_l2tpd" name="user_name_l2tpd" size="20" maxLength="20" /> </td>
						</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>

						<td width="40%" id="l2tpdPassword">Password </td>
						<td><input type="password" maxLength="20" size="20" id="user_password_l2tpd" name="user_password_l2tpd" />
						  </td>
						</tr>
					</table>
				</li>
				<li class="w_text">
				<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
				<tr>
				<td colspan=2>

				<input type='button' id='l2tpd_table_add' onclick='to_submit("l2tpd_table", "add", -1, edit_idx, l2tpd_user_rule);' value='Add' />
				<input type="hidden" value="/local/advance/vpn_l2tpd.asp" name="service_url">
				</td>
				</tr>
				</table>
				</li>

              </fieldset><br />

			  <fieldset>
				<li class="space3"></li>

				<li class="title">
				<table border="0" cellpadding="0" cellspacing="0">
				<tr><td nowrap id="userRuleSum">User Rules Summary</td>
				</tr></table>
				</li>

				<%tbl_create_table("l2tpd_table", "l2tpd_user_rule", "1");%>

				<script type="text/javascript">
					tbl_setup_table("l2tpd_table", l2tpd_user_rule);
                </script>

              </fieldset><br />

            </ul>
          </div>
          </li>
    <center>
      <li class="table_button">
	  <input type="reset" value="Reset" id="VPNReset" name="reset" onClick="window.location.reload()" />
      <input type=hidden value="/local/advance/vpn_l2tpd.asp" name="l2tpd_url">

      </li>
    </center>
	</ul>
	</div>
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

