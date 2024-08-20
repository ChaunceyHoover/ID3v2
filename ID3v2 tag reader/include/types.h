#ifndef ID3v2_TYPE_DEFINITIONS
#define ID3v2_TYPE_DEFINITIONS

namespace id3v2 {
enum Error {
	OK = 0,
	CANT_READ_FILE,
	NO_HEADER,
	INVALID_ID3v2_FLAGS,
	END_OF_FRAMES,
	INVALID_FRAME_HEADER_FLAGS
};

struct ID3v2_Header {
	int major, revision;
	int id3_size;
	bool is_unsynchronisation, has_extended_header, has_experimental_indicator, has_footer;
};

struct ID3v2_Extended_Header {
	int extended_header_size;
};

struct ID3v2_Frame_Header {
	char frame_id[5];
	int frame_size;
	
	bool is_tag_alter_preservation, is_file_alter_preservation, is_read_only;
	bool is_grouping_identity, has_compression, is_encrypted,
		is_unsynchronized, has_data_length_indicator;
};

struct ID3v2_Frame {
	ID3v2_Frame_Header header;
	char *data;
};
} //namespace id3v2

#endif