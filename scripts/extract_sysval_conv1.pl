while(<>) {
	if(/^\s*#define\s+(SV_\S+)\s+/) {
		print "$1\n";
	}
}
