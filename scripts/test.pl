while(<>) {
	chop;
	if(/^\s*#define\s+(\S?\SM)_\S+\s+\S+\s*$/) {
		$type=$1;
		if(!defined($types{$type})) {
			print "$type\n";
			$types{$type}=1;
		}
	}
}