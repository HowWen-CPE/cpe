BEGIN {
	chn_count=0;
	chn_curr=0;
}
{
	if($1 == "Channel") {
		chn_no[chn_count] = $2;
		chn_freq[chn_count] = $4 * 1000;
		chn_count = chn_count + 1;
	}
}
/Current Frequency/ {
	chn_curr=$5;
	sub(/)/,"",chn_curr);
}
END {
	print chn_count, chn_curr;
	for(i=0;i<chn_count;i++) {
		print chn_no[i], chn_freq[i];
	}
}
