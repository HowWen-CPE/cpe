<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">

<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>

<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="/local/advance/table.js"></script>
<script type="text/javascript" src="images/common.js"></script>
<script type="text/javascript" src="images/code.js"></script>
<script type="text/javascript" src="images/svg-common.js"></script>

<script language="JavaScript" type="text/javascript">

Butterlate.setTextDomain("internet");

var lanIP = "<%getLanIp(); %>";
var lanMask="<% getLanNetmask(); %>";
var isMaxNum = "<% isMaximum(); %>";
var wanIP = "<%getWanIp(); %>";

var t;

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
		alert("Error. IP address is empty.");
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	if (isAllNum(field.value) == 0) {
		alert('It should be a [0-9] number.');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
	
	var patrn=/^(127)\.(0{1,3})\.(0{1,3})\.(1)$/;
	if (patrn.exec(field.value)) {
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	if (ismask) {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 0, 255)))
		{
			alert('IP adress format error.');
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	} else {
		if ((!checkRange(field.value, 1, 0, 223)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 1, 254)))
		{
			alert('IP adress format error.');
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	return true;
}

function checkNetMask(field)
{
        //(field.value != "0.0.0.0") && (field.value != "255.255.255.255")&&
	if ((field.value != "128.0.0.0") &&
			(field.value != "192.0.0.0") && (field.value != "224.0.0.0") &&
			(field.value != "240.0.0.0") && (field.value != "248.0.0.0") &&
			(field.value != "252.0.0.0") && (field.value != "254.0.0.0") &&
			(field.value != "255.0.0.0") && (field.value != "255.128.0.0") &&
			(field.value != "255.192.0.0") && (field.value != "255.224.0.0") &&
			(field.value != "255.240.0.0") && (field.value != "255.248.0.0") &&
			(field.value != "255.252.0.0") && (field.value != "255.254.0.0") &&
			(field.value != "255.255.0.0") && (field.value != "255.255.128.0") &&
			(field.value != "255.255.192.0") && (field.value != "255.255.224.0") &&
			(field.value != "255.255.240.0") && (field.value != "255.255.248.0") &&
			(field.value != "255.255.252.0") && (field.value != "255.255.254.0") &&
			(field.value != "255.255.255.0") && (field.value != "255.255.255.128") &&
			(field.value != "255.255.255.192") && (field.value != "255.255.255.224") &&
			(field.value != "255.255.255.240") && (field.value != "255.255.255.248") &&
			(field.value != "255.255.255.252") && (field.value != "255.255.255.254"))			
	{
		alert("Error. Invalid Subnet Mask.");
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
	return true;
}

/*function isValidDigt(time) {
	if (time == undefined)
		return false;

	if (time.value == "" || isFinite(time.value) == false)
	{
		return false;
	}
	switch (time.name) {
		case 'phase1_lifetime': 
		case 'phase2_lifetime':
			{
				if (time.value < 1200 && time.value > 86400)
					return false;
				break;
			}

		case 'dpd_interval':
			{
				if (time.value < 10 && time.value > 1200)
					return false;
				break;
			}

		case 'dpd_timeout':
			{
				if (time.value < 30 && time.value > 3600)
					return false;
				break;
			}
		default: 
			return false;

	}
	return true;
}*/

function checkTime() {

	if (E('adv').checked == true) {
		if(isAllNum(E('phase1_lifetime').value)==0){
			alert('Lifetime value should be digit.');
			return false;
		}
		if (E('phase1_lifetime').value =="" ||E('phase1_lifetime').value > 86400 ||E('phase1_lifetime').value < 1200) {
			alert('phase1 lifetime should be from 1200s to 86400s.');
			return false;
		}
		
		if(isAllNum(E('phase2_lifetime').value)==0){
			alert('Lifetime vlaue should be digit.');
			return false;
		}
		if (E('phase2_lifetime').value =="" ||E('phase2_lifetime').value > 86400 ||E('phase2_lifetime').value < 1200) {
			alert('phase2 lifetime should be from 1200s to 86400s.');
			return false;
		}
	}
	if (E('dpd_enable').checked == true) {
		if (E('dpd_interval').value == "" || E('dpd_interval').value > 1200 || E('dpd_interval').value < 10) {
			alert('DPD Interval should be from 10s to 1200s.');
			return false;
		}

		if (E('dpd_timeout').value == "" || E('dpd_timeout').value > 3600 || E('dpd_timeout').value < 30) {
			alert('DPD Timeout should be from 30s to 3600s.');
			return false;
		}
	}
	return true;
}
//check Remote subnet ip and current lan ip are on the same net 
function isSameNet(remote,lan,r_netmask,l_netmask){
      var  lanValue=atoi(lan, 1)*256*256*256 +atoi(lan, 2)*256*256+ atoi(lan, 3)*256+atoi(lan, 4);
      var  inputValue=atoi(remote.value, 1)*256*256*256 +atoi(remote.value, 2)*256*256+ atoi(remote.value, 3)*256+atoi(remote.value, 4);  
	
      var rnetmaskValue=atoi(r_netmask.value, 1)*256*256*256 +atoi(r_netmask.value, 2)*256*256+ atoi(r_netmask.value, 3)*256+atoi(r_netmask.value, 4); 
      var lnetmaskValue=atoi(l_netmask, 1)*256*256*256 +atoi(l_netmask, 2)*256*256+ atoi(l_netmask, 3)*256+atoi(l_netmask, 4); 
      var lan_tmp=lanValue&lnetmaskValue;
      var input_tmp=inputValue&rnetmaskValue;
      
        if( input_tmp == lan_tmp ){               
		return true;
	}else{               
		return false;
	}
}


function checkIpaddr() {

	if (checkIpAddr(E('local_inipaddr'), 0) == false) {
		alert('Local internal ip address error!');
		return false;
	}

	if (checkIpAddr(E('local_netmask'), 1) == false) {
		alert('Local netmask error!');
		return false;
	}
	return true;
}
var F; 

//Enter only 1-20 letters or digitals, which only can be a string that start with letter and include digital, "_", "."
function isRegisterUserName(s)
{
	var patrn=/^[a-zA-Z]{1}([a-zA-Z0-9]|[._]){0,19}$/;
	if (!patrn.exec(s)) 
		return false
	else
		return true
}

//Enter only 1-40 letters or digitals, which only can be a string that start with letter/digital and include digital, "-", "."
function isValidFqdnValue(s)
{
	var patrn=/^[a-zA-Z0-9]{1}([a-zA-Z0-9]|[.-]){0,39}$/;
	if (!patrn.exec(s)) 
		return false
	else
		return true
}

//^[A-Za-z0-9]+$
//Enter letters or digitals. The min length of preshared key is 8, and the max length is 40.
function isPassword(s)
{
	var patrn=/^[A-Za-z0-9_]+$/;
	if (!patrn.exec(s)) 
		return false
	else
		return true
}

function rule_list_check_dup(T, R, index, row, edit_index) {
    var name1 = R[row][index];
    var operation_index = edit_index;
	 
    for (var j = 0; j < row; j++) {
        /* _ENABLE */
       if (R[j][0] == 0) {
           continue;
       }
	if(j == operation_index)
           continue;
       var name2 = R[j][index];
       if (index == 0 && name1 == name2) {
              alert('Connection name repeats.');
	       return false;
	}
	else if (name1 == name2 && name2 == "fqdn") {
            var value_name1 = R[row][index + 1];
            var value_name2 = R[j][index + 1];
            if(index == 29){
       	if(value_name1 == value_name2){
                 alert('Remote FQDN identifier repeats.');
                 return false;
       	}
           }
           if(index == 31){
       	if(value_name1 == value_name2){
                 alert('Local FQDN identifier repeats.');
                 return false;
		}
           }
       }
    }
    return true;
}
function local_table_check(T, R, idx) {

	/* Check the rule num */
	/*if (parseInt(E(T+'_cur_rule_num').value) >= parseInt(E(T+"_max_rule_num").value)) {
		alert("The max number of the rules is "+E(T+"_max_rule_num").value+"!");
		return;
	}*/

	var tmp = R.clone();
	var i = parseInt(E(T+'_cur_rule_num').value);
	var s = local_generate_rule(T);
       tmp.move(i, i, s);

	/* Check the duplicate Conn Name */
	if (rule_list_check_dup(T, tmp, enum_ipsec._NAME, i, idx) == false) {
		return false;
	} 
	
	/* Check the duplicate REMOTEID Value */
	if (rule_list_check_dup("ipsec_table", tmp, enum_ipsec._REMOTEID_TYPE, i, edit_idx) == false) {
		return false;
	} 
	
	/* Check the duplicate LOCALID Value */
	if (rule_list_check_dup("ipsec_table", tmp, enum_ipsec._LOCALID_TYPE, i, edit_idx) == false) {
		return false;
	} 

	/* Check the duplictate VPN mode (Remote User) */
	/*if (rule_list_check_dup(T, tmp, enum_ipsec._MODE, i) == false) {
		return false;
	} */

	return true;
}

function local_rule_check() {
	if (E('name').value == '') {
		alert('Name is required.');
		return false;
	}
	if (isRegisterUserName(E('name').value) == false) {
		alert('Name error. The entered value only support 1-20 letters digits underline or dot, and start with letter.');
		return false;
	}
	if(E('remote_id_type').value=='fqdn'){
       	if (isValidFqdnValue(E('remote_id_value').value) == false) {
       		alert('FQDN value error. The entered value only support 1-40 letters digits hyphen or dot, and start with letter or digital.');
       		return false;
       	}
	}else{
		if (checkIpAddr(E('remote_id_value'), 0) == false) {
			alert('Remote identifier IP address format is error!');
			return false;
		}
	}
	if(E('local_id_type').value=='fqdn'){
       	if (isValidFqdnValue(E('local_id_value').value) == false) {
       		alert('FQDN value error. The entered value only support 1-40 letters digits hyphen or dot, and start with letter or digital.');
       		return false;
       	}
	}
	if (E('mode').value == 'net2net') {
		if (E('remote_gateway').value == '') {
			alert('Remote Gateway is required.');
			return false;
		}
		if (E('remote_inipaddr').value == '') {
			alert('Remote Subnet IP is required.');
			return false;
		}

		if (checkIpAddr(E('remote_gateway'), 0) == false) {
			alert('Remote Gateway ip address error!');
			return false;
		}

		if (checkIpAddr(E('remote_inipaddr'), 0) == false) {
			alert('Remote internal ip address error!');
			return false;
		}

		if (checkIpAddr(E('remote_netmask'), 1) == false) {
			alert('Remote netmask error!');
			return false;
		}
		if(isSameNet(E('remote_inipaddr'),lanIP,E('remote_netmask'),lanMask)){
			alert('Remote subnet IP should not the same as current LAN subnet IP!');
			return false;
		}
	}
    var form=document.vpn_ipsec_rule;
    if(form.dpd_enable.checked == true){                                    
        if (E('dpd_interval').value <= 1200 ||E('dpd_interval').value >= 10 ){  
                                                                                
        }else{                                                                  
                alert('DPD Interval error!');                                   
                return false;                                                   
        }                                                                       
                                                                                
        if (E('dpd_timeout').value <= 3600 ||E('dpd_timeout').value >= 30 ){    
                                                                                
        }else{                                                                  
           alert('DPD Timeout error!');                                       
           return false;                                                      
        }                                                                     
     }       
    
	if (E('ike_keymode').value == 'psk') {
		if (E('psk').value == '') {
			alert('Preshared Key is required!');
			return false;
		}
		if (isPassword(E('psk').value) == false) {
			alert('Enter letters digits or underline. Preshared Key length should be from 8 to 40!');
			return false;
		}
		var len = E('psk').value.length;
		if (len < 8 || len > 40) {
			alert('Preshared Key length should be from 8 to 40 digits letters or underline.');
			return false;
		}
	}
	if (checkTime() == false) 
		return false;

	if (checkIpaddr() == false) 
		return false;

	return true;
}

var default_rule = ['','1','net2net','0','wan0','<%nv_attr_get("lan_static_rule", 0, "ipaddr");%>','<%getLanNetmask();%>','','','255.255.255.0','1','psk','','','','0','main','','3600','28800','md5','3des','dh2','md5','3des','dh2','0','10','60','ipaddr','','ipaddr','<%getWanIp(); %>'];

var rule = default_rule;

function local_create_input_field(T, nth, r) {
	rule = (r != undefined) ? r : default_rule;
}

function local_pack_key_value_pair(F, T, P) {
	/* Important */
	tbl_append_element(F, 'ipsec_rule', '');

	var row_num = parseInt(E(T+'_cur_rule_num').value);

	tbl_append_element(F, P + "_num", row_num);

	for (var i = 0; i < row_num; i++) {
		tbl_append_element(F, P + i + '_name', ipsec_rule[i][enum_ipsec._NAME]);
		tbl_append_element(F, P + i + '_enable', ipsec_rule[i][enum_ipsec._ENABLE]);
		tbl_append_element(F, P + i + '_mode', ipsec_rule[i][enum_ipsec._MODE]);
		tbl_append_element(F, P + i + '_l2tp', ipsec_rule[i][enum_ipsec._L2TP]);
		tbl_append_element(F, P + i + '_local_extif', ipsec_rule[i][enum_ipsec._LOCAL_EXTIF]);
		tbl_append_element(F, P + i + '_local_inipaddr', ipsec_rule[i][enum_ipsec._LOCAL_INIPADDR]);
		tbl_append_element(F, P + i + '_local_netmask', ipsec_rule[i][enum_ipsec._LOCAL_NETMASK]);
		tbl_append_element(F, P + i + '_remote_gateway', ipsec_rule[i][enum_ipsec._REMOTE_GATEWAY]);
		tbl_append_element(F, P + i + '_remote_inipaddr', ipsec_rule[i][enum_ipsec._REMOTE_INIPADDR]);
		tbl_append_element(F, P + i + '_remote_netmask', ipsec_rule[i][enum_ipsec._REMOTE_NETMASK]);
		tbl_append_element(F, P + i + '_conn_init', ipsec_rule[i][enum_ipsec._CONN_INIT]);
		tbl_append_element(F, P + i + '_ike_keymode', ipsec_rule[i][enum_ipsec._IKE_KEYMODE]);
		tbl_append_element(F, P + i + '_psk', ipsec_rule[i][enum_ipsec._PSK]);
		tbl_append_element(F, P + i + '_rsa', ipsec_rule[i][enum_ipsec._RSA]);
		tbl_append_element(F, P + i + '_ca', ipsec_rule[i][enum_ipsec._CA]);
		tbl_append_element(F, P + i + '_adv', ipsec_rule[i][enum_ipsec._ADV]);
		tbl_append_element(F, P + i + '_phase1_mode', ipsec_rule[i][enum_ipsec._PHASE1_MODE]);
		tbl_append_element(F, P + i + '_phase1_auth', ipsec_rule[i][enum_ipsec._PHASE1_AUTH]);
		tbl_append_element(F, P + i + '_phase1_encrypt', ipsec_rule[i][enum_ipsec._PHASE1_ENCRYPT]);
		tbl_append_element(F, P + i + '_phase1_group', ipsec_rule[i][enum_ipsec._PHASE1_GROUP]);
		tbl_append_element(F, P + i + '_phase1_id', ipsec_rule[i][enum_ipsec._PHASE1_ID]);
		tbl_append_element(F, P + i + '_phase2_auth', ipsec_rule[i][enum_ipsec._PHASE2_AUTH]);
		tbl_append_element(F, P + i + '_phase2_encrypt', ipsec_rule[i][enum_ipsec._PHASE2_ENCRYPT]);
		tbl_append_element(F, P + i + '_phase2_group', ipsec_rule[i][enum_ipsec._PHASE2_GROUP]);
		tbl_append_element(F, P + i + '_phase1_lifetime', ipsec_rule[i][enum_ipsec._PHASE1_LIFETIME]);
		tbl_append_element(F, P + i + '_phase2_lifetime', ipsec_rule[i][enum_ipsec._PHASE2_LIFETIME]);
		tbl_append_element(F, P + i + '_dpd_enable', ipsec_rule[i][enum_ipsec._DPD_ENABLE]);
		tbl_append_element(F, P + i + '_dpd_interval', ipsec_rule[i][enum_ipsec._DPD_INTERVAL]);
		tbl_append_element(F, P + i + '_dpd_timeout', ipsec_rule[i][enum_ipsec._DPD_TIMEOUT]);
		tbl_append_element(F, P + i + '_remote_id_type', ipsec_rule[i][enum_ipsec._REMOTEID_TYPE]);
		tbl_append_element(F, P + i + '_remote_id_value', ipsec_rule[i][enum_ipsec._REMOTEID_VALUE]);
		tbl_append_element(F, P + i + '_local_id_type', ipsec_rule[i][enum_ipsec._LOCALID_TYPE]);
		tbl_append_element(F, P + i + '_local_id_value', ipsec_rule[i][enum_ipsec._LOCALID_VALUE]);
	}
}

function local_generate_rule(T) {
	/* rule[] is existing. r[] is temporary used. Therefore, when we check the
	 * original value of the input element, we need to use rule[] instead of
	 * r[].
	 */
	var r = new Array();

	r[enum_ipsec._NAME] =  E('name').value;
	r[enum_ipsec._ENABLE] = tbl_dewrap_enable('enable');
	r[enum_ipsec._MODE] = E('mode').value;

	r[enum_ipsec._L2TP] = tbl_dewrap_enable('l2tp');
	r[enum_ipsec._REMOTE_GATEWAY] = E('remote_gateway').value;
	r[enum_ipsec._REMOTE_INIPADDR] = E('remote_inipaddr').value;
	r[enum_ipsec._REMOTE_NETMASK] = E('remote_netmask').value;
	r[enum_ipsec._CONN_INIT] = tbl_dewrap_enable('conn_init');

	r[enum_ipsec._LOCAL_EXTIF] = (E('local_extif') != undefined) ? E('local_extif').value : rule[enum_ipsec._LOCAL_EXTIF];

	r[enum_ipsec._LOCAL_INIPADDR] = E('local_inipaddr').value;
	r[enum_ipsec._LOCAL_NETMASK] = E('local_netmask').value;

	r[enum_ipsec._IKE_KEYMODE] = E('ike_keymode').value;

	r[enum_ipsec._RSA] = (E('rsa') != undefined) ? E('rsa').value : rule[enum_ipsec._RSA];
	r[enum_ipsec._CA] = (E('ca') != undefined) ? E('ca').value : rule[enum_ipsec._CA];

	r[enum_ipsec._PSK] = E('psk').value;

	r[enum_ipsec._REMOTEID_TYPE] = E('remote_id_type').value;
	r[enum_ipsec._REMOTEID_VALUE] = E('remote_id_value').value;
	r[enum_ipsec._LOCALID_TYPE] = E('local_id_type').value;
	r[enum_ipsec._LOCALID_VALUE] = E('local_id_value').value;


	r[enum_ipsec._DPD_ENABLE] = tbl_dewrap_enable('dpd_enable');

	if (r[enum_ipsec._DPD_ENABLE] == '1') {
		r[enum_ipsec._DPD_INTERVAL] = E('dpd_interval').value;
		r[enum_ipsec._DPD_TIMEOUT] = E('dpd_timeout').value;
	} else {
		r[enum_ipsec._DPD_INTERVAL] = rule[enum_ipsec._DPD_INTERVAL];
		r[enum_ipsec._DPD_TIMEOUT] = rule[enum_ipsec._DPD_TIMEOUT];
	}
	r[enum_ipsec._ADV] = tbl_dewrap_enable('adv');
	if (r[enum_ipsec._ADV] == '1') {
		r[enum_ipsec._PHASE1_MODE] = E('phase1_mode').value;
		r[enum_ipsec._PHASE1_AUTH] = E('phase1_auth').value;
		r[enum_ipsec._PHASE1_ENCRYPT] = E('phase1_encrypt').value;
		r[enum_ipsec._PHASE1_GROUP] = tbl_dewrap('phase1_group');
		r[enum_ipsec._PHASE1_ID] = E('phase1_id').value;
		r[enum_ipsec._PHASE2_AUTH] = E('phase2_auth').value;
		r[enum_ipsec._PHASE2_ENCRYPT] = E('phase2_encrypt').value;
		r[enum_ipsec._PHASE2_GROUP] = E('phase2_group').value;
		r[enum_ipsec._PHASE1_LIFETIME] = E('phase1_lifetime').value;
		r[enum_ipsec._PHASE2_LIFETIME] = E('phase2_lifetime').value;
	} else {
		r[enum_ipsec._PHASE1_MODE] = rule[enum_ipsec._PHASE1_MODE];
		r[enum_ipsec._PHASE1_AUTH] = rule[enum_ipsec._PHASE1_AUTH];
		r[enum_ipsec._PHASE1_ENCRYPT] = rule[enum_ipsec._PHASE1_ENCRYPT];
	    r[enum_ipsec._PHASE1_GROUP] = rule[enum_ipsec._PHASE1_GROUP];
	    r[enum_ipsec._PHASE1_ID] = rule[enum_ipsec._PHASE1_ID];
	    r[enum_ipsec._PHASE2_AUTH] = rule[enum_ipsec._PHASE2_AUTH];
	    r[enum_ipsec._PHASE2_ENCRYPT] = rule[enum_ipsec._PHASE2_ENCRYPT];
	    r[enum_ipsec._PHASE2_GROUP] = rule[enum_ipsec._PHASE2_GROUP];
	    r[enum_ipsec._PHASE1_LIFETIME] = rule[enum_ipsec._PHASE1_LIFETIME];
	    r[enum_ipsec._PHASE2_LIFETIME] = rule[enum_ipsec._PHASE2_LIFETIME];
    }

    return r;
}

var enum_ipsec = {
    _NAME:0,
	_ENABLE:1,
	_MODE:2,
	_L2TP:3,
	_LOCAL_EXTIF:4,
	_LOCAL_INIPADDR:5,
	_LOCAL_NETMASK:6,
	_REMOTE_GATEWAY:7,
	_REMOTE_INIPADDR:8,
	_REMOTE_NETMASK:9,
	_CONN_INIT:10,
	_IKE_KEYMODE:11,
	_PSK:12,
	_RSA:13,
	_CA:14,
	_ADV:15,
	_PHASE1_MODE:16,
	_PHASE1_ID:17,
	_PHASE1_LIFETIME:18,
	_PHASE2_LIFETIME:19,
	_PHASE1_AUTH:20,
	_PHASE1_ENCRYPT:21,
	_PHASE1_GROUP:22,
	_PHASE2_AUTH:23,
	_PHASE2_ENCRYPT:24,
	_PHASE2_GROUP:25,
	_DPD_ENABLE:26,
	_DPD_INTERVAL:27,
	_DPD_TIMEOUT:28,
	_REMOTEID_TYPE:29,
	_REMOTEID_VALUE:30,
	_LOCALID_TYPE:31,
	_LOCALID_VALUE:32
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

function vpnModeChange()
{
   if (document.vpn_ipsec_rule.mode.options.selectedIndex == 0)
   {
        document.vpn_ipsec_rule.remote_gateway.disabled = false;
        document.vpn_ipsec_rule.remote_inipaddr.disabled = false;
        document.vpn_ipsec_rule.remote_netmask.disabled = false;
        document.vpn_ipsec_rule.conn_init.disabled = false;
   }
   else
   {
        document.vpn_ipsec_rule.remote_gateway.disabled = true;
        document.vpn_ipsec_rule.remote_inipaddr.disabled = true;
        document.vpn_ipsec_rule.remote_netmask.disabled = true;
        document.vpn_ipsec_rule.conn_init.disabled = true;
        document.vpn_ipsec_rule.remote_gateway.value = "";                                            
        document.vpn_ipsec_rule.remote_inipaddr.value = "";
   }
}

function localIdTypeInit()
{
   if (document.vpn_ipsec_rule.local_id_type.options.selectedIndex == 0)
   {
        document.vpn_ipsec_rule.local_id_value.disabled = true;
        document.vpn_ipsec_rule.local_id_value.value = wanIP;
   }
   else
   {
   		document.vpn_ipsec_rule.local_id_value.disabled = false;
   }

}

function localIdTypeChange()
{
   if (document.vpn_ipsec_rule.local_id_type.options.selectedIndex == 0)
   {
        document.vpn_ipsec_rule.local_id_value.disabled = true;
        document.vpn_ipsec_rule.local_id_value.value = wanIP;
   }
   else
   {
        document.vpn_ipsec_rule.local_id_value.disabled = false;
        document.vpn_ipsec_rule.local_id_value.value = "";
   }

}

function remoteIdTypeChange()
{
   if (document.vpn_ipsec_rule.remote_id_type.options.selectedIndex == 0)
   {
	    document.vpn_ipsec_rule.remote_id_value.value = "";
   }
   else
   {
        document.vpn_ipsec_rule.remote_id_value.disabled = false;
        document.vpn_ipsec_rule.remote_id_value.value = "";
   }

}

function radioChange()
{
   var form=document.vpn_ipsec_rule;
   if(form.adv.checked == true) {
       form.phase1_mode.disabled=false;
       form.phase1_lifetime.disabled=false;
       form.phase1_id.disabled=false;
       form.phase1_auth.disabled=false; 
       form.phase1_encrypt.disabled=false;
       form.phase1_group.disabled=false;
       form.phase2_lifetime.disabled=false;
       form.phase2_auth.disabled=false; 
       form.phase2_encrypt.disabled=false;
       form.phase2_group.disabled=false;	
   }else {
       form.phase1_mode.disabled=true;
       form.phase1_lifetime.disabled=true;
       form.phase1_id.disabled=true;
       form.phase1_auth.disabled=true; 
       form.phase1_encrypt.disabled=true;
       form.phase1_group.disabled=true;
       form.phase2_lifetime.disabled=true;
       form.phase2_auth.disabled=true; 
       form.phase2_encrypt.disabled=true;
       form.phase2_group.disabled=true;
   }
  document.vpn_ipsec_rule.local_inipaddr.value = lanIP;
  document.vpn_ipsec_rule.local_netmask.value = lanMask;
  
}

function dpdChange()
{
   var form=document.vpn_ipsec_rule;
   if(form.dpd_enable.checked == true){
       form.dpd_interval.disabled=false;
       form.dpd_timeout.disabled=false;	
   }else {
       form.dpd_interval.disabled=true;
       form.dpd_timeout.disabled=true;
   }
}

function local_fill_input_field() {

	//E('ipsec_table_seq').value = rule[enum_ipsec._RULE_NUM];

	E('name').value = rule[enum_ipsec._NAME];
	E('mode').value = rule[enum_ipsec._MODE];

	check_box_set('enable', rule[enum_ipsec._ENABLE]);

	if (rule[enum_ipsec._MODE] == 'rw_server') {
		check_box_set('l2tp', rule[enum_ipsec._L2TP]);
	}

	E('local_extif').value = rule[enum_ipsec._LOCAL_EXTIF];

	if ((rule[enum_ipsec._MODE] == 'net2net') ||
			(rule[enum_ipsec._MODE] == 'rw_server' && rule[enum_ipsec._L2TP] == '0')) {
		E('local_inipaddr').value = rule[enum_ipsec._LOCAL_INIPADDR];
		E('local_netmask').value = rule[enum_ipsec._LOCAL_NETMASK];
	}
	if (rule[enum_ipsec._MODE] == 'net2net') {
		E('remote_gateway').value = rule[enum_ipsec._REMOTE_GATEWAY];
		E('remote_inipaddr').value = rule[enum_ipsec._REMOTE_INIPADDR];
		E('remote_netmask').value = rule[enum_ipsec._REMOTE_NETMASK];
		check_box_set('conn_init', rule[enum_ipsec._CONN_INIT]);
	}

	E('ike_keymode').value = rule[enum_ipsec._IKE_KEYMODE];
	if (rule[enum_ipsec._IKE_KEYMODE] == 'psk') {
		E('psk').value = rule[enum_ipsec._PSK];
	} else if (rule[enum_ipsec._IKE_KEYMODE] == 'rsa') {
		E('rsa').value = rule[enum_ipsec._RSA];
	} else if (rule[enum_ipsec._IKE_KEYMODE] == 'ca') {
		E('ca').value = rule[enum_ipsec._CA];
	}

	E('remote_id_type').value = rule[enum_ipsec._REMOTEID_TYPE];
	E('remote_id_value').value = rule[enum_ipsec._REMOTEID_VALUE];
	E('local_id_type').value = rule[enum_ipsec._LOCALID_TYPE];
	E('local_id_value').value = rule[enum_ipsec._LOCALID_VALUE];

	check_box_set('dpd_enable', rule[enum_ipsec._DPD_ENABLE]);

	E('dpd_interval').value = rule[enum_ipsec._DPD_INTERVAL];
	E('dpd_timeout').value = rule[enum_ipsec._DPD_TIMEOUT];

	check_box_set('adv', rule[enum_ipsec._ADV]);

	E('phase1_mode').value = rule[enum_ipsec._PHASE1_MODE];
	E('phase1_id').value = rule[enum_ipsec._PHASE1_ID];
	E('phase1_lifetime').value = rule[enum_ipsec._PHASE1_LIFETIME];
	E('phase2_lifetime').value = rule[enum_ipsec._PHASE2_LIFETIME];
	E('phase1_auth').value = rule[enum_ipsec._PHASE1_AUTH];
	E('phase1_encrypt').value = rule[enum_ipsec._PHASE1_ENCRYPT];
	E('phase1_group').value = rule[enum_ipsec._PHASE1_GROUP];
	E('phase2_auth').value = rule[enum_ipsec._PHASE2_AUTH];
	E('phase2_encrypt').value = rule[enum_ipsec._PHASE2_ENCRYPT];
	E('phase2_group').value = rule[enum_ipsec._PHASE2_GROUP];
	radioChange();
	localIdTypeInit();
	vpnModeChange();
	dpdChange();
}

function ipsec_tbl_insert_tr(T, R, arr, from, to)
{

	if (E(T+'_rule_flag').value == '1') {
		if (to < 0 || to > parseInt(E(T+'_cur_rule_num').value)) {
			alert("Inserted position out of boundary");
			return;
		}
	} else {
		if (to < 0 || to >= parseInt(E(T+'_cur_rule_num').value)) {
			alert("Inserted position out of boundary");
			return;
		}
	}

	if (E(T+'_rule_flag').value == '1') {
		var i = parseInt(E(T+'_cur_rule_num').value) + 1;
		E(T+'_cur_rule_num').value = i;
	}

	R.move(from, to, arr);
}
var edit_idx;

function ipsec_tbl_del_tr(T, R, idx) {

	var i = parseInt(E(T+'_cur_rule_num').value) - 1;
	E(T+'_cur_rule_num').value = i;
	R.splice(idx, 1);
}


function doEditRule(idx) {
	local_create_input_field("ipsec_table", -1, ipsec_rule[idx]);
	local_fill_input_field(); 
	edit_idx = idx;

	E('ipsec_table_add').style.display = "none";
	E('ipsec_table_edit').style.display = "block";
}

function doDeleteRule(idx) {
	to_submit('ipsec_table', 'del', idx);
}

function edit_submit(edit_idx, R) {

	if (local_rule_check() == false) {
        return;
	}

	var tmp = R.clone();
	var i = parseInt(E("ipsec_table"+'_cur_rule_num').value);
	
	var s = local_generate_rule("ipsec_table");
	
       tmp.move(i, i, s);

	/* Check the duplicate Conn Name */
	if (rule_list_check_dup("ipsec_table", tmp, enum_ipsec._NAME, i, edit_idx) == false) {
		return false;
	} 

	/* Check the duplicate REMOTEID Value */
	if (rule_list_check_dup("ipsec_table", tmp, enum_ipsec._REMOTEID_TYPE, i, edit_idx) == false) {
		return false;
	} 
	
	/* Check the duplicate LOCALID Value */
	if (rule_list_check_dup("ipsec_table", tmp, enum_ipsec._LOCALID_TYPE, i, edit_idx) == false) {
		return false;
	} 
	//delete
	R.splice(edit_idx, 1);

	//add
	R.splice(edit_idx, 0, s);
	//clean
	edit_idx = '';

       local_pack_key_value_pair(F, "ipsec_table", "ipsec_rule");

	showWebMessage(2, "");
	F.submit();
}
function to_submit(T, act, idx) {

	if (act == "del") {
	     ipsec_tbl_del_tr(T, ipsec_rule, idx); 
	     local_pack_key_value_pair(F, "ipsec_table", "ipsec_rule");
	     showWebMessage(2, "");
	     F.submit();
            return;
	}
       //Limit Max setting rule, the Max num can be setting by configuration ipsec_rule_max.
       if(isMaxNum == "1" )
       {
    	    alert("The setting rules exceed maximum!");
	    return false;
	}

	if (local_rule_check() == false) {
        return;
	}

	if (local_table_check(T, ipsec_rule, idx) == false) {
		return;
	}

	if (act == "add") {
		var s = local_generate_rule(T);
		ipsec_tbl_insert_tr(T, ipsec_rule, s, parseInt(E(T+'_cur_rule_num').value), parseInt(E(T+'_cur_rule_num').value));
	}

       local_pack_key_value_pair(F, "ipsec_table", "ipsec_rule");

	showWebMessage(2, "");
	F.submit();
}

function initTranslation(){
	var e = document.getElementById("ipsecRule");
	e.innerHTML = _("ipsec Rule");

	e = document.getElementById("connectName");
	e.innerHTML = _("connect Name");
	e = document.getElementById("ruleEnable");
	e.innerHTML = _("rule Enable");
	e = document.getElementById("vpnMode");
	e.innerHTML = _("vpn Mode");
	
	e = document.getElementById("localMask");
	e.innerHTML = _("local Mask");
	e = document.getElementById("local_iniIp");
	e.innerHTML = _("local Ip");
	e = document.getElementById("remoteGateway");
	e.innerHTML = _("remote Gateway");
	
	e = document.getElementById("remoteSubIp");
	e.innerHTML = _("remote Sub Ip");
	e = document.getElementById("remoteMask");
	e.innerHTML = _("remote Mask");
	e = document.getElementById("connectInit");
	e.innerHTML = _("connect Init");
	
	e = document.getElementById("keyMode");
	e.innerHTML = _("key Mode");
	
	e = document.getElementById("preshareKey");
	e.innerHTML = _("preshare Key");
	e = document.getElementById("dpdEnable");
	e.innerHTML = _("dpd Enable");

	e = document.getElementById("dpdInterval");
	e.innerHTML = _("dpd Interval");
	e = document.getElementById("dpdtime");
	e.innerHTML = _("dpd time");
	e = document.getElementById("advancedOpt");
	e.innerHTML = _("advanced Opt");
	e = document.getElementById("ph1Mode");
	e.innerHTML = _("ph1 Mode");
	e = document.getElementById("ph1Lifetime");
	e.innerHTML = _("ph1 Lifetime");
	e = document.getElementById("ph1Authen");
	e.innerHTML = _("ph1 Authen");
	e = document.getElementById("ph1Encryption");
	e.innerHTML = _("ph1 Encryption");
	e = document.getElementById("ph1GroupKey");
	e.innerHTML = _("ph1 Group Key");
	
	e = document.getElementById("ph2Lifetime");
	e.innerHTML = _("ph2 Lifetime");
	e = document.getElementById("ph2Authen");
	e.innerHTML = _("ph2 Authen");
	e = document.getElementById("ph2Encryption");
	e.innerHTML = _("ph2 Encryption");
	e = document.getElementById("ph2GroupKey");
	e.innerHTML = _("ph2 GroupKey");

	e = document.getElementById("ipsec_table_add");
	e.value = _("ipsec Table Add");
	e = document.getElementById("ipsec_table_add1");
	e.value = _("ipsec Table Add");
	e = document.getElementById("ipsec_table_edit");
	e.value = _("ipsec Table Add");

	e = document.getElementById("rule_num");
	e.innerHTML = _("routing Number");
	
	e = document.getElementById("applicationAppRuleSum");
	e.innerHTML = _("AppRuleSum");
	e = document.getElementById("applicationAppRuleSum2");
	e.innerHTML = _("AppRuleSum");
	e = document.getElementById("IpsecConnName");
	e.innerHTML = _("Ipsec Conn Name");
	e = document.getElementById("IpsecRuleEnable");
	e.innerHTML = _("Ipsec Rule Enable");	
	e = document.getElementById("IpsecRemoteGW");
	e.innerHTML = _("Ipsec Remote GW");
	e = document.getElementById("IpsecSubIpNetmask");
	e.innerHTML = _("Ipsec SubIp Netmask");
	e = document.getElementById("IpsecPhase1");
	e.innerHTML = _("Ipsec Phase1");
	e = document.getElementById("IpsecPhase2");
	e.innerHTML = _("Ipsec Phase2");
	e = document.getElementById("applicationTdModify");
	e.innerHTML = _("application TdModify");
	
	e = document.getElementById("remoteIDType");
	e.innerHTML = _("Remote ID Type");
	e = document.getElementById("remoteIDValue");
	e.innerHTML = _("Remote Identifier");
	e = document.getElementById("localIDType");
	e.innerHTML = _("Local ID Type");
	e = document.getElementById("localIDValue");
	e.innerHTML = _("Local Identifier");
	e = document.getElementById("remote_ipaddr");
	e.innerHTML = _("Remote WAN IP");
	e = document.getElementById("local_ipaddr");
	e.innerHTML = _("Local WAN IP");	
	e = document.getElementById("IPSECNote1_0");
	e.innerHTML = _("DHCP Mode note1_0");	
	e = document.getElementById("IPSECNote1_1");
	e.innerHTML = _("IPsec note1_1");	
	
	e = document.getElementById("net2net_mode");
	e.innerHTML = _("IPsec net2net_mode");	
	e = document.getElementById("rw_mode");
	e.innerHTML = _("IPsec rw_mode");	
	e = document.getElementById("main_mode");
	e.innerHTML = _("IPsec main_mode");	
	e = document.getElementById("aggre_mode");
	e.innerHTML = _("IPsec aggre_mode");	
	
	e = document.getElementById("cancel_button");
	e.value = _("general reset");
	
	
} 

function init() {
    F = document.getElementById("vpn_ipsec_rule");
	initTranslation();
	local_fill_input_field(); 
}
</script>
<body onload="init();">

  <form id="vpn_ipsec_rule" name="vpn_ipsec_rule" action="/goform/vpn_ipsec_rule" method="post">
    <script type="text/javascript">
      do_hidden_elements("ipsec_action")
    </script>
    <% tbl_get("ipsec_rule"); %>
    <% get_ipsec_status(); %>
    <div id="table">
      <ul>
        <li class="table_content">
          <div class="data">
            <ul>
              <li class="title">
                <table border="0" cellpadding="0" cellspacing="0">
                  <tr>
                    <td nowrap id="ipsecRule">
                      IPsec Policy Settings
                    </td>
                  </tr>
                </table>
              </li>

              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="300" id="connectName">
                      Connection Name
                    </td>
                    <td>
                      <input id='name' name='name' size='20' maxlength='20' onblur='valid_name(this, "Connection Name", 1)'/ >
                    </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="300" id="ruleEnable">
                      Rule Enable
                    </td>
                    <td>
                      <input id='enable' name='enable' type='checkbox' /> 
                    </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="300" id="vpnMode">
                      VPN Mode
                    </td>
                    <td>

                      <select id='mode' name='mode' onChange="vpnModeChange()">
                        <option id='net2net_mode' value='net2net'>
                          Site-to-Site
                        </option>
                        <option id='rw_mode' value='rw_server'>
                          Remote User
                        </option>
                      </select>
                    </td>

                  </tr>
                </table>
              </li>
              <li class="w_text" style="display: none">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>

                    <td width="300" id="routingGateway">
                      L2TP Enable
                    </td>
                    <td>
                      <input id='l2tp' name='l2tp' type='checkbox' />
                    </td>
                  </tr>
                </table>
              </li>
              <li class="w_text" style="display: none">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr >
                    <td width="300" id="local_extif_name">
                      Local External Interface
                    </td>
                    <td>
                      <select id='local_extif' name='local_extif'>
                        <option value='wan0'>
                          Ethernet
                        </option>
                      </select>
                    </td>
                  </tr>
                </table>
              </li>

              <li class="w_text" >
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="300" id="localMask">
                      Local Netmask
                    </td>
                    <td>
                      <input id='local_netmask' name='local_netmask' size='17' maxlength='30' disabled/>
                    </td>

                  </tr>
                </table>
              </li>

              <li class="w_text" >
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>

                    <td width="300" id="local_iniIp">
                      Local Internal IP Address
                    </td>
                    <td>
                      <input id='local_inipaddr' name='local_inipaddr' size='17' maxlength='30' disabled/>
                    </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="300" id="remoteGateway">
                      Remote Gateway
                    </td>
                    <td>
                      <input id='remote_gateway' name='remote_gateway' size='17' maxlength='30'onchange='valid_name(this, "Remote Gateway")' />
                    </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="300" id="remoteSubIp">
                      Remote Subnet IP
                    </td>
                    <td>
                      <input id='remote_inipaddr' name='remote_inipaddr' size='17' maxlength='30'/>
                    </td>

                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>

                    <td width="300" id="remoteMask">
                      Remote Netmask
                    </td>
                    <td>
                      <input id='remote_netmask' name='remote_netmask' size='17' maxlength='30'/>
                    </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="300" id="connectInit">
                      Connection Initiation
                    </td>
                    <td>
                      <input id='conn_init' name='conn_init' type='checkbox'>
                    </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="300" id="keyMode">
                      IKE Key Mode
                    </td>
                    <td>
                      <select id='ike_keymode' name='ike_keymode'>
                        <option value='psk'>
                          PSK
                        </option>
                      </select>
                    </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="300" id="preshareKey">
                      Preshared Key
                    </td>
                    <td>
                      <input type='password' id='psk' name='psk' size='25' maxlength='40'/>
                    </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="300" id="remoteIDType">
                      Remote ID Type
                    </td>
                    <td>
                      <select id='remote_id_type' name='remote_id_type' onChange="remoteIdTypeChange()">
                        <option id='remote_ipaddr' value='ipaddr'>
                          Remote WAN (internet) IP
                        </option>
                        <option id='remote_fqdn' value='fqdn'>
                          FQDN
                        </option>
                      </select>
                    </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="300" id="remoteIDValue">
                     Remote Identifier
                    </td>
                    <td>
                      <input id='remote_id_value' name='remote_id_value' size='25' maxlength='40'/>
                    </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="300" id="localIDType">
                      Local ID Type
                    </td>
                    <td>
                      <select id='local_id_type' name='local_id_type' onChange="localIdTypeChange()">
                        <option id='local_ipaddr' value='ipaddr'>
                          Local WAN (internet) IP
                        </option>
                        <option id='local_fqdn' value='fqdn'>
                          FQDN
                        </option>
                      </select>
                    </td>
                  </tr>
                </table>
              </li>              
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="300" id="localIDValue">
                     Local Identifier
                    </td>
                    <td>
                      <input id='local_id_value' name='local_id_value' size='25' maxlength='40'/>
                    </td>
                  </tr>
                </table>
              </li>

              <li class="space3">
              </li>

              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="300" id="dpdEnable">
                      DPD Enable
                    </td>
                    <td>
                      <input id='dpd_enable' name='dpd_enable' type='checkbox' onclick="dpdChange()" />
                    </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                 
                      <td width="300" id="dpdInterval">
                        DPD Interval
                      </td>
                      <td>
                        <input id='dpd_interval' name='dpd_interval' size='7' maxlength='4'>
                        (10 ~ 1200)
                      </td>
					  </tr>
                </table>
              </li>

              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                      <td width="300" id="dpdtime">
                        DPD Timeout
                      </td>
                      <td>
                        <input id='dpd_timeout' name='dpd_timeout' size='7' maxlength='4'>
                        (30 ~ 3600)
                  </tr>
                </table>
              </li>
 
              <li class="space3">
              </li>


              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td width="300" id="advancedOpt">
                      Advanced Options
                    </td>
                    <td>
                      <input id='adv' name='adv' type='checkbox' onclick="radioChange()"/>
                    </td>
                  </tr>
                </table>
              </li>

              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>

                          <td width="300" id="ph1Mode">
                            Phase 1 Mode
                          </td>
                          <td>
                            <select id='phase1_mode' name='phase1_mode'>
                              <option id='main_mode' value='main'>
                                Main
                              </option>
                              <option id='aggre_mode' value='aggressive'>
                                Aggressive
                              </option>
                            </select>
                          </td>
                  </tr>
                </table>
              </li>
              <li class="w_text" style="display: none">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                          <td width="300" id="ph1Id">
                            Phase 1 ID
                          </td>
                          <td>
                            <input id='phase1_id' name='phase1_id' size='17' maxlength='15'>
                          </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                          <td width="300" id="ph1Lifetime">
                            Phase 1 Lifetime
                          </td>
                          <td >
                            <input id='phase1_lifetime' name='phase1_lifetime' size='5' maxlength='5'>
                            (1200 ~ 86400)
                          </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                          <td width="300" id="ph1Authen">
                            Phase 1 Authentication
                          </td>
                          <td>
                            <select id='phase1_auth' name='phase1_auth'>
                              <option value='md5'>
                                MD5
                              </option>
                              <option value='sha1'>
                                SHA1
                              </option>
                            </select>
                          </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                          <td width="300" id="ph1Encryption">
                            Phase 1 Encryption
                          </td>
                          <td>
                            <select id='phase1_encrypt' name='phase1_encrypt'>
                              <option value='des'>
                                DES
                              </option>
                              <option value='3des'>
                                3DES
                              </option>
                              <option value='aes128'>
                                AES-128
                              </option>
                              <option value='aes192'>
                                AES-192
                              </option>
                              <option value='aes256'>
                                AES-256
                              </option>
                            </select>
                          </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                          <td width="300" id="ph1GroupKey">
                            Phase 1 Group Key Management
                          </td>
                          <td>
                            <select id='phase1_group' name='phase1_group'>
                              <option value='dh1'>
                                DH1
                              </option>
                              <option value='dh2'>
                                DH2
                              </option>
                              <option value='dh5'>
                                DH5
                              </option>
                            </select>
                          </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                          <td width="300" id="ph2Lifetime">
                            Phase 2 Lifetime
                          </td>
                          <td >
                            <input id='phase2_lifetime' name='phase2_lifetime' size='5' maxlength='5'>
                            (1200 ~ 86400)
                          </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                          <td width="300" id="ph2Authen">
                            Phase 2 Authentication
                          </td>
                          <td>
                            <select id='phase2_auth' name='phase2_auth'>
                              <option value='md5'>
                                MD5
                              </option>
                              <option value='sha1'>
                                SHA1
                              </option>
                            </select>
                          </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                          <td width="300" id="ph2Encryption">
                            Phase 2 Encryption
                          </td>
                          <td>
                            <select id='phase2_encrypt' name='phase2_encrypt'>
                              <option value='null'>
                                Null
                              </option>
                              <option value='des'>
                                DES
                              </option>
                              <option value='3des'>
                                3DES
                              </option>
                              <option value='aes128'>
                                AES-128
                              </option>
                              <option value='aes192'>
                                AES-192
                              </option>
                              <option value='aes256'>
                                AES-256
                              </option>
                            </select>
                          </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                          <td width="300" id="ph2GroupKey">
                            Phase 2 Group Key Management (PFS)
                          </td>
                          <td>
                            <select id='phase2_group' name='phase2_group'>
                              <option value='none'>
                                None
                              </option>
                              <option value='dh1'>
                                DH1
                              </option>
                              <option value='dh2'>
                                DH2
                              </option>
                              <option value='dh5'>
                                DH5
                              </option>
                            </select>
                          </td>

                  </tr>
                </table>
              </li>

              <li class="title">
				<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr>
                    <td colspan=2>
					<input id="ipsec_table_add" onclick="to_submit('ipsec_table', 'add', -1);" type="button" value='Add'/>
					<input style="display: none" id="ipsec_table_edit" onclick="edit_submit(edit_idx, ipsec_rule);" type="button" value='Add'/>
                    </td>
                  </tr>
                </table>
              </li>
              <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                   <tr>
                     <td><span class="i_note" id="IPSECNote1_0">Note:</span></td>
                   </tr>
                  </table>
                </li>                
                <li class="w_text2">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                   <tr>
                    <td class="i_note_a" id="IPSECNote1_1">
                      Site-to-Site: In this mode, when the setting LAN IP address is in the same network segment as remote subnet, the corresponding IPsec rule will be disabled.
                    </td>
                  </tr>
                 </table>
              </li>
              <li class="space3">
              </li>              
<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="applicationAppRuleSum">IPsec Rules Summary</td>
                   </tr></table>
</li>
<li class="space"></li>
<li class="w_text2">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><table width="100%" cellpadding="0" cellspacing="0" >
<tr>
<td height="26" colspan="10"><table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
<td background="images/table_top_center.gif"class="table_title" id="applicationAppRuleSum2"> IPsec Rules Summary<span class="cTitle"></span></td>
<td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
</tr>
</table></td>
</tr>           

                        <tr>
                          <td width="5%" align=center valign="top">
                            <span class="top_left" id="rule_num">
                              #
                            </span>
                          </td>
                          <td width="20%" align=center style="white-space:nowrap">
                            <font class="top_font">
                              <span id="IpsecConnName">
                              Connection Name
                              </span>

                            </font>
                          </td>
                          <td width="10%" align=center style="white-space:nowrap">
                            <font class="top_font">
                              <span id="IpsecRuleEnable">
                               Rule Enable
                              </span>
                              
                            </font>
                          </td>
                          <td width="15%" align=center style="white-space:nowrap">
                            <font class="top_font">
                              <span id="IpsecRemoteGW">
                              Remote Gateway
                              </span>

                            </font>
                          </td>
                          <td width="25%" align=center style="white-space:nowrap">
                            <font class="top_font">
                              <span id="IpsecSubIpNetmask">
                              
                              Remote Subnet IP
                                /
                                Netmask
                                </span>
                            </font>
                          </td>
                          <td width="5%" align=center style="white-space:nowrap">
                            <font class="top_font">
                              <span id="IpsecPhase1">
                              
                              Phase 1
                            </span>
                            </font>
                          </td>
                          <td width="5%" align=center style="white-space:nowrap">
                            <font class="top_font">
                              <span id="IpsecPhase2">
                             
                              Phase 2
                             </span>
                            </font>
                          </td>
                          <td width="15%" align=center style="white-space:nowrap">
                            <font class="top_right">
                              <span id="applicationTdModify">
                               Modify
                              </span>
                            </font>
                          </td>
                        </tr>
                        <% showIpsecVpnRulesASP(); %>
                          <tr>
                            <td height="5" colspan="10">
                              <table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
                                <tr>
                                  <td width="5" height="5"><img src="images/table_bottom_left.gif" width="5" height="5" /></td>
                                  <td height="5" background="images/table_bottom_center.gif"><img src="images/table_bottom_center.gif" width="1" height="5" /></td>
                                  <td width="5"><img src="images/table_bottom_right.gif" width="5" height="5" /></td>
                                </tr>
                              </table>
                            </td>
                          </tr>

                      </table>
                    </td>
                  </tr>
                </table>
                <li>
                </li>
                <li>
                </li>
            </ul>
          </div>
          </li>
          <center>
            <li class="table_button">
              <input type="reset" value="Cancel" id="cancel_button" name="reset" onClick="window.location.reload()">
              <input type=hidden value="/local/advance/vpn_ipsec.asp" name="ipsec_url">
		<input type='hidden' id='ipsec_table_add1' onClick='tbl_show_input("ipsec_table", 1, ipsec_rule);' value='Add' />
		<input type='hidden' id='ipsec_table_delete' onClick='tbl_del_tr("ipsec_table", ipsec_rule);' value='Delete' />
		<input type='hidden' id='ipsec_table_modify' onclick='tbl_show("ipsec_table", 0, ipsec_rule);' value='Modify' />
                <%tbl_create_table("ipsec_table", "ipsec_rule", "1");%>
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
