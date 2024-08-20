#ifndef ID3v2_TAG_PARSER
#define ID3v2_TAG_PARSER

#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "types.h"

namespace id3v2 {
class ID3v2 {
private:
	std::ifstream m_audio_file{};
	Error has_valid_header(char *t_buffer);
	Error has_valid_frame_header(char *t_buffer);
	Error parse_frame_header(char *t_buffer, ID3v2_Frame_Header &header);

public:
	ID3v2_Header m_header{};
	ID3v2_Extended_Header m_extended_header{};

	std::map<std::string, std::string> m_tags{};
	std::vector<ID3v2_Frame> m_frames{};

	ID3v2(std::string t_path);

	Error init();
	Error parse_header(char *t_buffer);
	Error parse_extended_header(char *t_buffer);
	Error parse_frames();
};
} //namespace ID3v2

#endif