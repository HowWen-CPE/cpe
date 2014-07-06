
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

function checkIpRange(str, num, min, max)
{
    var k = 0;
	for (var i=0; i<str.length; i++) {
	  if (str.charAt(i) == '.')
		 k = k+1;
	  continue;
	}
	if(k > 3){
	    alertbottom("Error. IP address is not valid.");
        return false;
	}
	d = atoi(str, num);
	if (d > max || d < min )
		return false;
	return true;
}

/*
 *@FUNC: check entered IP is class E or not 
 *@PARAM: 
 *str:Entered IP Address
 *num:which segment of IP address
 *@RETURN:
 *true:IP address is class E or IP address
 *format error.
 *false:IP address is not class E.
*/
function checkClassEAddr(str, num)
{
    var k = 0;
	for (var i=0; i<str.length; i++) {
	  if (str.charAt(i) == '.')
		 k = k+1;
	  continue;
	}
	if(k > 3){
	  alertbottom("Error. IP address is not valid.");
      return true;
	}
	d = atoi(str, num);
	if (d > 239 && d < 255)
	{
	  alertbottom("Error. Class E IP address cannot be used.");
	  return true;
	}      
	return false;
}

/*
 *@FUNC: check entered IP is multicast 
 *address or not 
 *@PARAM: 
 *str:Entered IP Address
 *num:which segment of IP address
 *@RETURN:
 *true:IP address is multicast address or 
 *IP address format error.
 *false:IP address is not multicast address.
*/
function checkMulticastAddr(str, num)
{
    var k = 0;
	for (var i=0; i<str.length; i++) {
	  if (str.charAt(i) == '.')
		 k = k+1;
	  continue;
	}
	if(k > 3){
	  alertbottom("Error. IP address is not valid.");
      return true;
	}
	d = atoi(str, num);
	if (d >223 && d <240 )
	{
	  alertbottom("Error. IP address is multicast address.");
	  return true;
	}      
	return false;
}

/*
 *@FUNC: check entered IP is lookback 
 *address or not 
 *@PARAM: 
 *str:Entered IP Address
 *num:which segment of IP address
 *@RETURN:
 *true:IP address is lookback address or 
 *IP address format error.
 *false:IP address is not lookback address.
*/
function checkLoopbackAddr(str, num)
{
    var k = 0;
	for (var i=0; i<str.length; i++) {
	  if (str.charAt(i) == '.')
		k = k+1;
	  continue;
	}
	if(k > 3){
	   alertbottom("Error. IP address is not valid.");
       return true;
	}
	d = atoi(str, num);
	if (d == 127 )
	{
	       alertbottom("Error. IP address is loopback address.");
		return true;
	}
      
	return false;
}

/*
 *@FUNC: check entered value is number or not. 
 *@PARAM: 
 *str:Entered value
 *@RETURN:
 *0:Entered value is number. 
 *1:Entered value is not number.
*/
function isDigit(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9'))
			continue;
		return 0;
	}
	return 1;
}

/*
 *@FUNC: check entered IP is dot separated or not. 
 *@PARAM: 
 *str:Entered IP Address
 *@RETURN:
 *0:IP address value is number. 
 *1:IP address is not number.
*/
function isDotSeparatedIp(str)
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
	    alertbottom("Error. IP address is empty.");
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
	if (field.value == "255.255.255.255") {
	    alertbottom("Error. IP address is broadcast address.");
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
	if (isDotSeparatedIp(field.value) == 0) {
		alertbottom('It should be a [0-9] number.');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
	if (checkLoopbackAddr(field.value,1) || checkMulticastAddr(field.value,1)) {		
		field.value = field.defaultValue;
		field.focus();
		return false;
	}	
	if (checkClassEAddr(field.value,1)) {		
		field.value = field.defaultValue;
		field.focus();
		return false;
	}	
	if (ismask) {
		if ((!checkIpRange(field.value, 1, 0, 255)) ||
				(!checkIpRange(field.value, 2, 0, 255)) ||
				(!checkIpRange(field.value, 3, 0, 255)) ||
				(!checkIpRange(field.value, 4, 0, 255)))
		{
			alertbottom('IP adress format error.');
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	else {
		if ((!checkIpRange(field.value, 1, 1, 223)) ||
				(!checkIpRange(field.value, 2, 0, 255)) ||
				(!checkIpRange(field.value, 3, 0, 255)) ||
				(!checkIpRange(field.value, 4, 1, 254)))
		{
			alertbottom('IP adress format error.');
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	return true;
}

function checkLanGatewayIpAddr(field, ismask)
{
	if (field.value == "0.0.0.0") {
	    return true;
	}
	if (field.value == "") {
	    field.value = "0.0.0.0";
	    return true;
	}
	if (isDotSeparatedIp(field.value) == 0) {
		alertbottom('It should be a [0-9] number.');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	if (checkLoopbackAddr(field.value,1) || checkMulticastAddr(field.value,1)) {		
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
	if (ismask) {
		if ((!checkIpRange(field.value, 1, 0, 255)) ||
				(!checkIpRange(field.value, 2, 0, 255)) ||
				(!checkIpRange(field.value, 3, 0, 255)) ||
				(!checkIpRange(field.value, 4, 0, 255)))
		{
			alertbottom('IP adress format error.');
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	else {
		if ((!checkIpRange(field.value, 1, 1, 223)) ||
				(!checkIpRange(field.value, 2, 0, 255)) ||
				(!checkIpRange(field.value, 3, 0, 255)) ||
				(!checkIpRange(field.value, 4, 1, 254)))
		{
			alertbottom('IP adress format error.');
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	return true;
}
