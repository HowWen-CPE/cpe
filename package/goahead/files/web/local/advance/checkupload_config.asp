<html>
<meta http-equiv="Pragma" content="no-cache">
<head>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">Butterlate.setTextDomain("admin");
</script>
<script>
var n=1;
function waitdownload()
{
	var flag="<% getGetUploadConfigFlag(); %>";
	var tag= "<% getConfigOversize(); %>";
	if(tag == 1)
	{
			parent.upload_message=_('configuration size is too large to write down');
			self.location.href = '/local/advance/settings_gordon.asp';
	}
}
</script></head>
<body onLoad="waitdownload()">
</body>
</html>
