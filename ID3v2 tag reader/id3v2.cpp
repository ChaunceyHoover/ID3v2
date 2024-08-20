#include "id3v2.h"

using namespace id3v2;

static int bufferToSyncSafeInt(char *t_buffer) {
	return int(
		((t_buffer[0] & 0x7F) << 24) |
		((t_buffer[1] & 0x7F) << 16) |
		((t_buffer[2] & 0x7F) <<  8) |
		 (t_buffer[3] & 0x7F)
	);
}

Error ID3v2::has_valid_header(char *t_buffer) {
	std::string id3TagCheck = std::string(t_buffer, 3);

	// Ensure header is up to standards https://id3.org/id3v2.4.0-structure
	if (id3TagCheck != "ID3") {
		// The first three bytes of the tag are always "ID3", to indicate 
		// that this is an ID3v2 tag
		return NO_HEADER;
	} else if (t_buffer[3] == 255 || t_buffer[4] == 255) {
		// Version or revision will never be $FF.
		return INVALID_ID3v2_FLAGS;
	} else if ((t_buffer[5] & 0b00001111) != 0) {
		// All the other flags MUST be cleared. If one of these undefined flags
		// are set, the tag might not be readable for a parser that does not
		// know the flags function.
		return INVALID_ID3v2_FLAGS;
	}

	return OK;
};

Error ID3v2::parse_header(char *t_buffer) {
	// Ensure first 3 bits are "ID3" (49 44 33) and other flags are
	// correctly formatted
	Error header_validation = has_valid_header(t_buffer);
	if (header_validation != OK) {
		return header_validation;
	}

	// Get version from 4th and 5th bytes
	m_header.major = (int)t_buffer[3];
	m_header.revision = (int)t_buffer[4];

	// Get header flags
	m_header.is_unsynchronisation = t_buffer[5] >> 8;
	m_header.has_extended_header = t_buffer[5] >> 7;
	m_header.has_experimental_indicator = t_buffer[5] >> 6;
	m_header.has_footer = t_buffer[5] >> 5;

	// Get ID3v2 tag size
	char id3_size_buffer[4];
	
	memcpy(id3_size_buffer, t_buffer + 6, 4);
	int id3_tag_size = bufferToSyncSafeInt(id3_size_buffer);
	id3_tag_size -= m_header.has_footer ? 20 : 10;
	m_header.id3_size = id3_tag_size;

	return OK;
}

Error ID3v2::parse_extended_header(char *t_buffer) {
	return OK;
}

Error ID3v2::has_valid_frame_header(char *t_buffer) {
	if ((t_buffer[8] & 0b10001111) != 0 || (t_buffer[9] & 0b10110000) != 0) {
		// In the frame header the size descriptor is followed by two flag
		// bytes. All unused flags MUST be cleared.
		return INVALID_FRAME_HEADER_FLAGS;
	}

	return OK;
}

Error ID3v2::parse_frame_header(char *t_buffer, ID3v2_Frame_Header &header) {
	Error frame_header_validation = has_valid_frame_header(t_buffer);
	if (frame_header_validation != OK) {
		return frame_header_validation;
	}

	memcpy(header.frame_id, t_buffer, 4);
	header.frame_id[4] = '\0';

	char frame_size_buffer[4];
	memcpy(frame_size_buffer, t_buffer + 4, 4);
	header.frame_size = bufferToSyncSafeInt(frame_size_buffer) - 10; // does not include header

	char frame_flags[2];
	memcpy(frame_flags, t_buffer + 8, 2);

	header.is_tag_alter_preservation = frame_flags[0] >> 7;
	header.is_file_alter_preservation = frame_flags[0] >> 6;
	header.is_read_only = frame_flags[0] >> 5;

	header.is_grouping_identity = frame_flags[1] >> 7;
	header.has_compression = frame_flags[1] >> 4;
	header.is_encrypted = frame_flags[1] >> 3;
	header.is_unsynchronized = frame_flags[1] >> 2;
	header.has_data_length_indicator = frame_flags[1] >> 1;

	return OK;
}

Error ID3v2::parse_frames() {
	char frame_header_buffer[10];
	m_audio_file.read(frame_header_buffer, 10);

	ID3v2_Frame_Header frame_header{};
	Error frame_header_validation = parse_frame_header(frame_header_buffer, frame_header);
	if (frame_header_validation != OK) {
		return frame_header_validation;
	}

	ID3v2_Frame test{ frame_header };
	test.data = new char[frame_header.frame_size];
	m_audio_file.read(test.data, frame_header.frame_size);

	m_frames.push_back(test);

	return OK;
}

Error ID3v2::init() {
	if (!m_audio_file || !m_audio_file.is_open()) {
		return CANT_READ_FILE;
	}

	// Check if file has ID3v2 header
	char header_buffer[10];
	m_audio_file.read(header_buffer, 10);
	Error result = parse_header(header_buffer);

	// If any error found so far, ID3v2 header either not found or is invalid
	if (result != OK) {
		return result;
	}

	if (m_header.has_extended_header) {
		char extended_header_size_buffer[4];
		m_audio_file.read(extended_header_size_buffer, 4);
		int extended_header_size = bufferToSyncSafeInt(extended_header_size_buffer);
		m_extended_header.extended_header_size = extended_header_size;

		// TODO: Come back and get optional info. For now, skip over it since it's
		// not required for reading tags
		m_audio_file.seekg(10 + extended_header_size);
	}

	parse_frames();

	return OK;
}

ID3v2::ID3v2(std::string t_path) {
	m_audio_file = std::ifstream(t_path, std::ios::in | std::ios::binary);
}