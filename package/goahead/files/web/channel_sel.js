function getSelectSelected(selectbox)
{       
	return selectbox.options[selectbox.selectedIndex].value;
} 

function removeAllFromTheList(list){
	while(list.length>0)
		list.options[0] = null;
	return true;
}

function addToTheList(list,str,selected){
	var currentLength = list.length;
	list.length++;
	list.options[currentLength] = new Option(str, str);

	return list.options[currentLength].selected=selected;
}

function addValueTextToList(list,optValue,optText,optSelected){
	var currentLength = list.length;
	list.length++;
	list.options[currentLength] = new Option(optText,optValue);

	return list.options[currentLength].selected=optSelected;
}

function SetChannelIdx(value)
{
	var i;
	var channel_table;
	channel_table = document.wireless_basic.sz11gChannel.options;

	if(0 != value && value != document.wireless_basic.sz11gChannel.value ) {
		return false;	
	}

	for(i = 0; i < channel_table.length; i++)
	{
		if(channel_table[i].value == value)
		{
			channel_table[i].selected = true;
			return true;
		}
	}
	return false;
}

function SetExtIdx(index)
{
	var pos = document.wireless_basic.n_extcha.length;
	if (0==pos) {
		return false;
	}

	if(useExtChan() != true) {
		return false;
	}

	if(eval(index)<wireless_basic.n_extcha.length) {
		document.wireless_basic.n_extcha.options[index].selected=true;
	}

	return true;
}

function useExtChan()
{
	/* Can not set HT20 HT40 */
	if(document.wireless_basic.n_bandwidth.disabled == true)
	{  
		document.wireless_basic.n_extcha.disabled = true;
		return false;
	}

	/* HT40 selected */
	if(eval(document.wireless_basic.n_bandwidth.value) == 1)
	{  
		document.wireless_basic.n_extcha.disabled = false;
		return true;
	}
	else
	{  
		document.wireless_basic.n_extcha.disabled = true;
		return false;
	}
}

function refreshExtensionChanList(chanTab, selChan, ext_u, ext_l){
	var ret=false;
	var idx=0;
	var been_selected=false;
	removeAllFromTheList(document.wireless_basic.n_extcha);

	/* if SmartSelect selected, no extension channels */
	if(eval(selChan) == 0)
		return false;

	if(!(eval(document.wireless_basic.n_bandwidth.value) == 1))
		return false;

	for(idx=0; idx<chanTab.length-1; idx++){
		if(eval(ext_u) && ((eval(selChan) - 4) == eval(chanTab[idx][2])))
		{  
			tmp = addValueTextToList(document.wireless_basic.n_extcha, "-1", chanTab[idx][0], chanTab[idx][1]);
			been_selected = been_selected||tmp;
			ret = true;
			continue;
		}
		if(eval(ext_l) && ((eval(selChan) + 4) == eval(chanTab[idx][2])))
		{  
			tmp = addValueTextToList(document.wireless_basic.n_extcha, "1", chanTab[idx][0], chanTab[idx][1]);
			been_selected = been_selected||tmp;
			ret = true;
			break;
		}
	}
	if(!been_selected) {
		if(document.wireless_basic.n_extcha.length>0) {
			document.wireless_basic.n_extcha.options[0].selected=true;
		}
	}
	return ret;
}

function refreshChanList(channel)
{
	var index = 0;
	var been_selected=false;
	removeAllFromTheList(document.wireless_basic.sz11gChannel);

	//
	addValueTextToList(document.wireless_basic.sz11gChannel, "0", "Auto Channel Selection", "");

	for(i=0;i<channel.length-1;i++) {
		//HT20
		if(eval(document.wireless_basic.n_bandwidth.value) == 0) {
			tmp = addValueTextToList(document.wireless_basic.sz11gChannel,
					channel[i][2], channel[i][0], channel[i][1]=="SELECTED");
			been_selected = been_selected||tmp;
		}
		//HT40
		else if(eval(document.wireless_basic.n_bandwidth.value) == 1) {
			if(eval(channel[i][3]) == 1 || eval(channel[i][4]) == 1) {
				tmp = addValueTextToList(document.wireless_basic.sz11gChannel,
						channel[i][2], channel[i][0], channel[i][1]=="SELECTED");
				been_selected = been_selected||tmp;
			}
		}
		//HT80
		else if(eval(document.wireless_basic.n_bandwidth.value) == 2) {
			if(eval(channel[i][3]) == 1 && eval(channel[i][4]) == 1) {
				tmp = addValueTextToList(document.wireless_basic.sz11gChannel,
						channel[i][2], channel[i][0], channel[i][1]=="SELECTED");
				been_selected = been_selected||tmp;
			}
		}
	}

	if(!been_selected) {
		index = document.wireless_basic.sz11gChannel.length-1; 
		document.wireless_basic.sz11gChannel.options[0].selected=true;
	}
}

function refreshExtChannel()
{
	var i;
	var channel;
	var select_channel;
	var chan_idx = 0;

	/* enable / disable extension channel according to radion band selection */
	if(useExtChan() != true)
	{
		removeAllFromTheList(document.wireless_basic.n_extcha);
		return false;
	}

	switch(i=getSelectSelected(document.wireless_basic.wirelessmode)){
		case '1':
		case '4':
		case '6':
			break;
		case '9'://11bgn
			channel = channelBGN;
			break;
		case '8'://11an
		case '14'://11ac
			if(eval(document.wireless_basic.n_bandwidth.value) == 2) {
				channel = channelANHT80;
			}
			else {
				channel = channelAN;
			}
			break;
		default:
			alert("Error: Unknown Wireless Mode ("+i+")");
			return false;
	}
	
	select_channel = eval(document.wireless_basic.sz11gChannel.selectedIndex);

	for(i = 0; i < channel.length - 1; i++)
	{
		if(//!(eval(document.wireless_basic.n_bandwidth.value) == 1) || 
			(eval(channel[i][3]) == 1 || 
			 eval(channel[i][4]) == 1))
			chan_idx++;

		if(chan_idx == select_channel)
			break;
	}

	/* refresh extension channel list */
	if(chan_idx == select_channel)
	{
		if(refreshExtensionChanList(channel, channel[i][2], channel[i][3], channel[i][4]) != true)
			document.wireless_basic.n_extcha.disabled = true;
	}
	else /* channel SmartSelect in HT40 */
	{
		removeAllFromTheList(document.wireless_basic.n_extcha);
		document.wireless_basic.n_extcha.disabled = true;
	}

	return true;
}


function updateChannel(){
	tmpWMDisabled=document.wireless_basic.wirelessmode.disabled;
	tmpCHDisabled=document.wireless_basic.sz11gChannel.disabled;
	document.wireless_basic.wirelessmode.disabled=true;
	document.wireless_basic.sz11gChannel.disabled=true;


	switch(i=getSelectSelected(document.wireless_basic.wirelessmode)){
		case '1':
		case '4':
		case '6':
			break;
		case '9'://11b/g/n
			refreshChanList(channelBGN);
			break;
		case '8'://11an
		case '14'://11ac
			if(eval(document.wireless_basic.n_bandwidth.value) == 2) {
				refreshChanList(channelANHT80);
			}
			else {
				refreshChanList(channelAN);
			}
			break;
		default:
			alert("Error: Unknown Wireless Mode ("+i+")");
			return false;
	}
	document.wireless_basic.wirelessmode.disabled=tmpWMDisabled;
	document.wireless_basic.sz11gChannel.disabled=tmpCHDisabled;
}
