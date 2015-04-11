$old="";
while(<>) {
	chop;
	if($_ ne $old) {
		if($old ne "") {
			print "\telse ";
		}
		else {
			print "#define str2msg(a) \t     ";
		}
		print "if(!stricmp(a,\"$_\"))";
		for($l=0;$l<6-int((length())/4);$l++) {
			print "\t";
		}
		print "msg=$_; \\\n";
		$old=$_;
		
	}
}
print "\telse return INVALID_ROUTINE;\n";

