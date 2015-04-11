while(<>) {
	if(/^\s*#define\s+(WM_\S+)\s+/) {
		print "$1\n";
	}
	if(/^\s*#define\s+(EM_\S+)\s+/) {
		print "$1\n";
	}
	if(/^\s*#define\s+(SBM_\S+)\s+/) {
		print "$1\n";
	}
	if(/^\s*#define\s+(STM_\S+)\s+/) {
		print "$1\n";
	}
	if(/^\s*#define\s+(BM_\S+)\s+/) {
		print "$1\n";
	}
	if(/^\s*#define\s+(SM_\S+)\s+/) {
		print "$1\n";
	}
	if(/^\s*#define\s+(DM_\S+)\s+/) {
		print "$1\n";
	}
	if(/^\s*#define\s+(MM_\S+)\s+/) {
		print "$1\n";
	}
	if(/^\s*#define\s+(CBM_\S+)\s+/) {
		print "$1\n";
	}
	if(/^\s*#define\s+(LM_\S+)\s+/) {
		print "$1\n";
	}
	if(/^\s*#define\s+(TM_\S+)\s+/) {
		print "$1\n";
	}
}
