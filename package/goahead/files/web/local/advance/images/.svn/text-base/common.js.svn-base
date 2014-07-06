/* For IE9 */
function getBrowserVersion() {  
    var browser = {};  
    var userAgent = navigator.userAgent.toLowerCase();  
    var s;  
    (s = userAgent.match(/msie ([\d.]+)/))  
        ? browser.ie = s[1]  
        : 0;  
    if (browser.ie == "9.0") return true;
    else return false;
}
if (getBrowserVersion())
    if (typeof Range.prototype.createContextualFragment == "undefined") {
        Range.prototype.createContextualFragment = function(html) {
            var doc = this.startContainer.ownerDocument;
            var container = doc.createElement("div");
            container.innerHTML = html;
            var frag = doc.createDocumentFragment(), n;
            while ( (n = container.firstChild) ) {
                frag.appendChild(n);
            }
            return frag;
        };
}

Array.prototype.swap=function(a, b)
{
    var tmp=this[a];
    this[a]=this[b];
    this[b]=tmp;
}

Array.prototype.insert=function(pos, v) {
    if(pos < 0 || pos > this.length) {
        return;
    }

    for (var i = this.length - 1; i >= pos; i--) {
        this[i+1] = this[i];
    }

    this[pos] = v;
};

Array.prototype.move=function(from, to, v) {
    if (from < to) {
        for (var i = from + 1; i <= to; i++) {
            this[i-1] = this[i];
        }
    } else if (from > to) {
        for (var i = from - 1; i >= to; i--) {
            this[i+1] = this[i];
        }
    }
            
    this[to] = v;
};

Array.prototype.clone = function() {
	return this.slice(0);
};

ie4 = ((navigator.appName == "Microsoft Internet Explorer") && (parseInt(navigator.appVersion) >= 4 ))
ns4 = ((navigator.appName == "Netscape") && (parseInt(navigator.appVersion) < 6 ))
ns6 = ((navigator.appName == "Netscape") && (parseInt(navigator.appVersion) >= 6 ))

// 0.0.0.0
var ZERO_NO = 1;	// 0x0000 0001
var ZERO_OK = 2;	// 0x0000 0010
// x.x.x.0
var MASK_NO = 4;	// 0x0000 0100
var MASK_OK = 8;	// 0x0000 1000
// 255.255.255.255
var BCST_NO = 16;	// 0x0001 0000
var BCST_OK = 32;	// 0x0010 0000

var SPACE_NO = 1;
var SPACE_OK = 2;

function setCookie(c_name,value,expiredays)
{
    var exdate=new Date();
    exdate.setDate(exdate.getDate()+expiredays);
    document.cookie=c_name+ "=" +escape(value)+
        ((expiredays==null) ? "" : ";expires="+exdate.toUTCString());
}

function choose_enable(en_object) {
	if(!en_object)	return;
	en_object.disabled = false;			// netscape 4.x can not work, but 6.x can work

	if(!ns4)
		en_object.style.backgroundColor = "";	// netscape 4.x have error
}
function choose_disable(dis_object) {
	if(!dis_object)	return;
	dis_object.disabled = true;

	if(!ns4)
		dis_object.style.backgroundColor = "#e0e0e0";
}
function ignoreSpaces(string) {
	var temp = "";

	string = '' + string;
	splitstring = string.split(" ");
	for(i = 0; i < splitstring.length; i++) {
		temp += splitstring[i];
	}

	return temp;
}
function check_space(I,M1){
	M = unescape(M1);
	for(i=0 ; i<I.value.length; i++){
		ch = I.value.charAt(i);
		if(ch == ' ') {
			alert(M +' is not allowed to contain any space character!');
			I.value = I.defaultValue;	
			return false;
		}
	}

	return true;
}
function isascii(I,M) {
	for(i=0 ; i<I.value.length; i++){
		ch = I.value.charAt(i);
		if(ch < ' ' || ch > '~'){
			alert(M +' has illegal ascii code!');
			I.value = I.defaultValue;	
			return false;
		}
	}

	return true;
}

function isxdigit(I,M) {
	for(i=0 ; i<I.value.length; i++){
		ch = I.value.charAt(i).toLowerCase();
		if(ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'f') {
				
		} else {
			alert(M +' has illegal hexadecimal digits!');
			I.value = I.defaultValue;	
			return false;
		}
	}

	return true;
}

function trim_unused_ws(v) {
	var b = v.replace(/^\s*/,"");
    var c = b.replace(/\s*$/,"");
    return c;
}

function valid_reserved_word(I, M) {
    if (I.value.indexOf('^') != -1 || 
        I.value.indexOf('|') != -1 ||
        I.value.indexOf('\'') != -1 ||
        I.value.indexOf('"') != -1) {
        alert(M + 'contains illegal characters');
        I.value = '';
        return false;
    }
}

function valid_reserved_word2(I, M) {
    if (I.value.indexOf('|') != -1 ||
        I.value.indexOf('\'') != -1 ||
        I.value.indexOf('"') != -1) {
        alert(M + ': |, \' and " are illegal');
        I.value = '';
        return false;
    }
}
function valid_number(I, M) {
    I.value = trim_unused_ws(I.value);
    
	for(i = 0; i < I.value.length; i++){
		ch = I.value.charAt(i);
		if(ch < '0' || ch > '9') {
			alert(M +': ' + I.value + ' is not a number');
			I.value = '';
			return false;
		}
	}

	return true;
}

function valid_range(I, start, end, M) {
    if (I.value == '') {
        return true;
    }
    if (valid_number(I, M) == false) {
        return false;
    }
	d = parseInt(I.value);	
	if (d > end || d < start) {		
		alert(M + ': ' + d + ' is out of range ['+ start + ' - ' + end +']');
        I.value = '';
        return false;
	}
    return true;
}

function valid_hwaddr(I, M) {
    if (I.value == '') {
        return true;
    }

    var str = trim_unused_ws(I.value);
    I.value = str.toUpperCase();

    var mac_pat = /^([0-9A-F]{2}:){5}([0-9A-F]{2})$/
    if (!mac_pat.test(I.value)) {
        alert(M + ': ' + I.value + ' is not a MAC address');
        I.value = '';
        return false;
    }                     
}

function valid_name(I,M,flag) {
    I.value = trim_unused_ws(I.value);
	isascii(I,M);
    valid_reserved_word(I, M);

	if(flag & SPACE_NO){
		check_space(I,M);
	}
}

function valid_pppoe(I,M,flag) {
    I.value = trim_unused_ws(I.value);
	isascii(I,M);
    valid_reserved_word2(I, M);

	if(flag & SPACE_NO){
		check_space(I,M);
	}
}

function valid_ipaddr(I, M) {
    I.value = trim_unused_ws(I.value);
    var ipaddr = I.value;
    var re = /^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$/;
    if (ipaddr == '') {
        return true;
    }
    if (re.test(ipaddr)) {
        var parts = ipaddr.split(".");
        if (parseInt(parseFloat(parts[0])) == 0) {
           
            alert(M + ': ' + ipaddr + ' is not an IP address');
            I.value = '';
            return false;
        }
        for (var i=0; i<parts.length; i++) {
            if (parseInt(parseFloat(parts[i])) > 255) {
                alert(M + ': ' + ipaddr + ' is not an IP address');
                I.value = '';
                return false;
            }
        }
        return true;
    } else {
        alert(M + ': ' + ipaddr + ' is not an IP address');
        I.value = '';
        return false;
    }
}
/* Inconsistent code. Modify it when we rearrange the pages wl_*.asp. */
function valid_ip(F,N,M1,flag){
	var m = new Array(4);
	M = unescape(M1);

	for(i=0;i<4;i++)
		m[i] = eval(N+"_"+i).value

	if(m[0] == 127 || m[0] == 224){
		alert(M+" value is illegal!");
		return false;
	}

	if(m[0] == "0" && m[1] == "0" && m[2] == "0" && m[3] == "0"){
		if(flag & ZERO_NO){
			alert(M+' value is illegal!');
			return false;
		}
	}

	if((m[0] != "0" || m[1] != "0" || m[2] != "0") && m[3] == "0"){
		if(flag & MASK_NO){
			alert(M+' value is illegal!');
			return false;
		}
	}

	return true;
}

function valid_ip_gw(F,I,N,G) {
	var IP = new Array(4);
	var NM = new Array(4);
	var GW = new Array(4);
	
	for(i=0;i<4;i++)
		IP[i] = eval(I+"_"+i).value
	for(i=0;i<4;i++)
		NM[i] = eval(N+"_"+i).value
	for(i=0;i<4;i++)
		GW[i] = eval(G+"_"+i).value

	for(i=0;i<4;i++){
		if((IP[i] & NM[i]) != (GW[i] & NM[i])){
			alert("IP address and gateway are not at the same subnet!");
			return false;
		}
	}
	if((IP[0] == GW[0]) && (IP[1] == GW[1]) && (IP[2] == GW[2]) && (IP[3] == GW[3])){
		alert("IP address and gateway can't be same!");
		return false;
	}
	
	return true;
}

function W(s)
{
    document.write(s);
}

function E(e)
{
    return (typeof(e) == 'string') ? document.getElementById(e) : e;
}

function N(F, e) {
    return F.elements[e];
}

function pad(n)
{
    n = n.toString();
    while (n.length < 2) n = '0' + n;
    return n;
}

function sel_enable_objs(F, objs) {
    for (var i = 0; i < objs.length; i++) {
        if (N(F, objs[i].name)) {
            if (!N(F, objs[i].name).type && N(F, objs[i].name).length) {
                /* Other than that type is text. */
                for (var j = 0; j < N(F, objs[i].name).length; j++) {
                    N(F, objs[i].name)[j].disabled = false;
                }
            } else {
                N(F, objs[i].name).disabled = false;
            }
        } else if (E(objs[i].id)) {
            E(objs[i].id).disabled = false;
        }
    }
}

function sel_disable_objs(F, objs) {
    for (var i = 0; i < objs.length; i++) {
        if (N(F, objs[i].name)) {
            if (!N(F, objs[i].name).type && N(F, objs[i].name).length) {
                /* Other than that type is text. */
                for (var j = 0; j < N(F, objs[i].name).length; j++) {
                    N(F, objs[i].name)[j].disabled = true;
                }
            } else {
                N(F, objs[i].name).disabled = true;
            }
        } else if (E(objs[i].id)) {
            E(objs[i].id).disabled = true;
        }
    }
}

/* TODO: change naming as sel_change_show_objs */
function sel_change(v, check, F, objs) {
    if (v == check) {
        sel_enable_objs(F, objs);
    } else {
        sel_disable_objs(F, objs);
    }
}

/* TODO: change naming as nsel_change_show_objs */
function nsel_change(v, check, F, objs) {
    if (v != check) {
        sel_enable_objs(F, objs);
    } else {
        sel_disable_objs(F, objs);
    }
}
function get_page_width() {
    var pageWidth = 1280;
    if(document.body) {
        if (document.body.scrollWidth) {
            pageWidth = document.body.scrollWidth;
        } else if(document.body.offsetWidth ) {
            pageWidth = document.body.offsetWidth;
        }
    }
    return pageWidth;
}
function get_page_height() {
    var pageHeight = 1024;
    if(document.body) {
        if (document.body.scrollHeight) {
            pageHeight = document.body.scrollHeight;
        } else if(document.body.offsetHeight ) {
            pageHeight = document.body.offsetHeight;
        }
    }
    return pageHeight;
}

var http_request = false;
var flag = 0;
function ajax_request_post(url, parameters, postaction) {
    http_request = false;
    if (window.XMLHttpRequest) { // Mozilla, Safari,...
        http_request = new XMLHttpRequest();
        if(navigator.userAgent.indexOf("MSIE")>0){
                flag = 3;//IE8
        }else{                                                           
                flag = 1;                                          
        }
        if (http_request.overrideMimeType) {
            http_request.overrideMimeType('text/html');
        }
    } else if (window.ActiveXObject) { // IE6
		flag = 2;
        try {
            http_request = new ActiveXObject("Msxml2.XMLHTTP");
        } catch (e) {
            try {
                http_request = new ActiveXObject("Microsoft.XMLHTTP");
            } catch (e) {}
        }
    }
    if (!http_request) {
        alert('Cannot create XMLHTTP instance');
        return false;
    }
    if (postaction == undefined) {
        http_request.onreadystatechange = ajax_postaction;
    } else {
        http_request.onreadystatechange = postaction;
    }
    http_request.open('POST', url, true);
    http_request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    http_request.send(parameters);
}
var force_display_return_timer = null, reset_force_flag_timer = null;
function ajax_postaction() {
	/* Cancel return to login page, 2013-4-11  Ricardo-------------> 
	if(flag == '1' && http_request.getResponseHeader("Content-Type") != null){ //Mozilla, Safari,...
		window.location.href="/index.asp";
		return;
	} 
	else if(flag == '2' && http_request.getResponseHeader("Content-Type") != ''){ // IE6
		window.location.href="/index.asp";
		return;
	}else if(flag == '3' && http_request.getResponseHeader("Content-Type") != ''){//IE8
		window.location.href="/index.asp";
		return;
	}
	<----------------------------------------end 2013-4-11*/
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			var result = http_request.responseText;
			var ret = result.split('\n');
			var message = '';
			var i;
			for (i = 0; i < ret.length; i++) {
				var pair = ret[i].split('='); 
				if (pair[0] == "message") {
					message = pair[1];
				} else if (pair[0] == 'name') {
					name = pair[1];
				} else if (pair[0] == "return_value") {
					if (force_display_return_timer) {
						clearTimeout(force_display_return_timer);
						clearTimeout(reset_force_flag_timer);
					}
					if (pair[1] == '1') {
						display_countdown();
					} else if (pair[1] == '3') {
						var target  = document.getElementsByName(name)[0];
						target.value = message;
					} else {
						/* For IE6 problem, we postponed the display of
						* wait-window by 0.5 second. It causes the error
						* message on return-window be covered.
						*/
						var command = "display_return(" + pair[1] + ",'" + message + "');";
						setTimeout(command, 500);
					}
					return;
				}
			}
		}
		display_return(1);
	}
}
function ajax_parse_parameters(N, str) {
    if (N.tagName != "INPUT" && N.tagName != "SELECT") {
        return str;
    }    
    if (N.tagName == "INPUT") {
        if (N.type == "hidden") {
            str += N.name + "=";
            str += (N.value == undefined) ? "" : N.value; 
            str += "&";
        }
        if (N.type == "text") {
            str += N.name + "=" + N.value + "&";
        }
        if (N.type == "password") {
            str += N.name + "=" + N.value + "&";
        }
        if (N.type == "file") {
            str += N.name + "=" + N.value + "&";
        }
        if (N.type == "checkbox") {
            if (N.checked) {
                str += N.name + "=" + N.value + "&";
            } else {
                str += N.name + "=&";
            }
        }
        if (N.type == "radio") {
            if (N.checked) {
                str += N.name + "=" + N.value + "&";
            }
        }
    }   
    if (N.tagName == "SELECT") {
        str += N.name + "=" + N.options[N.selectedIndex].value + "&";
    }
    return str;
}        
       
function ajax_recur(F, str) {
    if (F.tagName == undefined) {
        return str;
    }
    var i;
    if (F.childNodes != undefined) {
        for (i = 0; i < F.childNodes.length; i++) {
            str = ajax_recur(F.childNodes[i], str);
        }
    }
    str = ajax_parse_parameters(F, str);
    return str;
}    

function js_ajax_preaction(F, ds, attr, rule, array_cnt, action, postaction) {
    var getstr = "";
    /* For common parameter */
    getstr = ajax_recur(F, getstr);
    if (action == undefined) {
        action = "apply.cgi";
    }
    if ((ds != undefined) && (array_cnt != undefined) && (array_cnt > 0)) {
        if (array_cnt == 1) {
            for (var i =0;i < ds.getCount(); i++) {
                for(var j = 0;j < attr.length; j++) {
                    var out = ds.getAt(i).get(attr[j]);
                    if (Ext.type(out) == 'boolean') {
                        out = (out == true) ? 1 : 0;
                    }
                    getstr = getstr + rule + i + '_' + attr[j] + '=' + out + '&';
                } 
            }
            getstr += rule + '_num=' + ds.getCount();
        }
        else {
            /* We may include more than one rule and data store in a
             * request. It is required to use multiple data stores. */
            for (var array_idx = 0; array_idx < array_cnt; array_idx++) {
                if (array_idx != 0) {
                    getstr = getstr + '&';
                }
                for (var i =0;i < ds[array_idx].getCount(); i++) {
                    for(var j = 0;j < attr[array_idx].length; j++) {
                        var out = ds[array_idx].getAt(i).get(attr[array_idx][j]);
                        if (Ext.type(out) == 'boolean') {
                            out = (out == true) ? 1 : 0;
                        }
                        getstr = getstr + rule[array_idx] + i + '_' + attr[array_idx][j] + '=' + out + '&';
                    } 
                }
                getstr += rule[array_idx] + '_num=' + ds[array_idx].getCount();
            }
        }
    }
    ajax_request_post(action, getstr, postaction);
}

function ajax_preaction(F, action, timer, postaction) {
    var getstr = "";

    /* For common parameter */
    getstr = ajax_recur(F, getstr);
    if (timer) {
        force_display_return_timer = setTimeout("force_display_return();", timer);
        reset_force_flag_timer = setTimeout("reset_force_flag();", timer + 2000);
    }
    if (action == undefined) {
        action = "apply.cgi";
    }
    ajax_request_post(action, getstr, postaction);
}

var http_req;
function widget_query(F) {
    http_req = false;
    if (window.XMLHttpRequest) { // Mozilla, Safari,...
        http_req = new XMLHttpRequest();
        if (http_req.overrideMimeType) {
            http_req.overrideMimeType('text/html');
        }
    } else if (window.ActiveXObject) { // IE
        try {
            http_req = new ActiveXObject("Msxml2.XMLHTTP");
        } catch (e) {
            try {
                http_req = new ActiveXObject("Microsoft.XMLHTTP");
            } catch (e) {}
        }
    }
    if (!http_req) {
        alert('Cannot create XMLHTTP instance');
        return false;
    }
    http_req.onreadystatechange = widget_postaction;
    http_req.open('POST', "apply.cgi", true);
    http_req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    http_req.send("action=Widget");
}

function to_update(num, type)
{
    F.type.value = type;
    ajax_preaction(F, "update.cgi", 6000);
    display_mask(F);
}
function to_action(num, type, action)
{
    var F = document.getElementsByName('form')[0];
    F.num.value = num;

    F.type.value = type;
    if (action == 'connect') {
        /* wanX_staus = 0: disconnected */
        ajax_preaction(F, "connect.cgi", 6000);
    } else if (action == 'disconnect') {
        /* wanX_staus = 0: connected */
        ajax_preaction(F, "disconnect.cgi", 6000);
    }
}

function def_get_index_by_id(D, ID) {
    for (var i = 0; i < D.length; i++) {
        if (D[i].id != ID) {
            continue;
        }
        return i;
    }
    return -1;
}

function rlist_check_dup_field(T, R, index) {
    var row_num = parseInt(E(T+'_cur_rule_num').value);
    for (var i = 0; i < row_num; i++) {
        /* _ENABLE == 0 */
        if (R[i][0] == 0) {
            continue;
        }
        var name1 = R[i][index];
        for (var j = i+1; j < row_num; j++) {
            /* _ENABLE */
            if (R[j][0] == 0) {
                continue;
            }
            var name2 = R[j][index];
            if (name1 == name2) {
                alert('"'+name1+'" repeats.');
                return false;
            }
        }
    }
    return true;
}

function dlist_set_value_by_id(L, D, ID, V)
{
    var index = def_get_index_by_id(D, ID);
    L[index] = V;
}

function def_set_value_by_id(D, ID, V)
{
    var index = def_get_index_by_id(D, ID);
    D[index].value = V;
}
function logout()
{
    setCookie("auth", "");
    window.location.href=""
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
