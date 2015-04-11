while(<>) {
	if(/^\s*#define\s+(QSV_\S+)\s+/) {
		print "$1\n";
	}
}
