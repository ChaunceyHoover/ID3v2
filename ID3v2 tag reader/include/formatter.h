#ifndef ID3v2_DEBUG_HELPER_FUNCS
#define ID3v2_DEBUG_HELPER_FUNCS

#include <string>

std::string translate_bool_to_yes_no(bool value) {
	return value ? "Yes" : "No";
}

std::string checkbox(bool value) {
	return value ? "[x]" : "[ ]";
}

std::string translate_tag(std::string tag) {
	if (tag == "TALB")
		return "Album";
	else if (tag == "TIT2")
		return "Title";

	return "(unknown)";
}

#endif