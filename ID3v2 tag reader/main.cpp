#define SHOW_ADDITIONAL_DEBUG

#define buffer_arr vector<unsigned char>

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "formatter.h"
#include "id3v2.h"

using namespace std;

string translate_encoding(int code) {
	if (code == 0)
		return "ISO-8859-1";
	else if (code == 1)
		return "UTF-16 encoded Unicode with BOM";
	else if (code == 2)
		return "UTF-16 encoded Unicode without BOM";
	else if (code == 3)
		return "UTF-8 encoded Unicode";

	return "(unknown)";
}

buffer_arr process_tags(buffer_arr buffer) {
	return buffer;
}

//void parse_file(string path) {
//	id3v2::ID3v2 test{ path };
//
//	ifstream input(path, ios::in | ios::binary);
//	if (!input || !input.is_open()) {
//		cerr << "File '" << path << "' does not open" << endl;
//		return;
//	}
//
//	// copy to buffer
//	buffer_arr original_buffer(istreambuf_iterator<char>(input), {});
//	buffer_arr header_buffer(original_buffer.begin(), original_buffer.begin() + 10);
//
//	// check for ID3v2 tags
//	string id3TagCheck = string(header_buffer.begin(), header_buffer.begin() + 3);
//	bool hasId3Tags = id3TagCheck == "ID3";
//	cout << "Does this MP3 have ID3v2 tags: " << translate_bool_to_yes_no(hasId3Tags) << endl;
//
//	if (!hasId3Tags) {
//		return;
//	}
//
//	// get ID3v2 version
//	int majorVersion = (int)header_buffer[3],
//		revision = (int)header_buffer[4];
//	cout << "ID3v2." << majorVersion << '.' << revision << endl;
//
//	// get flags
//	bool unsynchronisationFlag = header_buffer[5] >> 8,
//		 extendedHeaderFlag = header_buffer[5] >> 7,
//		 experimentalIndicatorFlag = header_buffer[5] >> 6,
//		 footerPresentFlag = header_buffer[5] >> 5,
//		 remainingFlagsCleared = (header_buffer[5] & 0xF) == 0;
//
//#ifdef SHOW_ADDITIONAL_DEBUG
//	cout << "Unsynchronisation applied to all frames: " << translate_bool_to_yes_no(unsynchronisationFlag) << endl
//		 << "Extended header present after this header: " << translate_bool_to_yes_no(extendedHeaderFlag) << endl
//		 << "Experimental indicator: " << translate_bool_to_yes_no(experimentalIndicatorFlag) << endl
//		 << "Footer present at end of tag: " << translate_bool_to_yes_no(footerPresentFlag) << endl;
//#endif
//
//	if (!remainingFlagsCleared) {
//		cerr << "Error: Unused ID3v2 flags (bits 5 through 8 of byte 6) are not cleared" << endl;
//		return;
//	}
//
//	// ID3v2 size
//	int id3Size = int(((header_buffer[6] & 0x7F) << 24) |
//			((header_buffer[7] & 0x7F) << 16) |
//			((header_buffer[8] & 0x7F) << 8) |
//			(header_buffer[9] & 0x7F));
//
//#ifdef SHOW_ADDITIONAL_DEBUG
//	cout << "ID3v2 size: " << (id3Size - (footerPresentFlag ? 20 : 10)) << endl;
//#endif
//
//	int extendedHeaderOffset = 0;
//	if (extendedHeaderFlag) {
//		extendedHeaderOffset = int(((original_buffer[10] & 0x7F) << 24) |
//				((original_buffer[11] & 0x7F) << 16) |
//				((original_buffer[12] & 0x7F) << 8) |
//				(original_buffer[13] & 0x7F));
//	}
//
//	// Jump over extended header since we don't need it
//	buffer_arr afterExtendedHeader(original_buffer.begin() + 10 + extendedHeaderOffset, original_buffer.end());
//
//	string frameId = string(afterExtendedHeader.begin(), afterExtendedHeader.begin() + 4);
//	cout << "FrameID: " << frameId << endl;
//
//	int size = int(((afterExtendedHeader[4] & 0x7F) << 24) |
//			((afterExtendedHeader[5] & 0x7F) << 16) |
//			((afterExtendedHeader[6] & 0x7F) << 8) |
//			(afterExtendedHeader[7] & 0x7F));
//
//#ifdef SHOW_ADDITIONAL_DEBUG
//	cout << "Frame size: " << size << endl;
//#endif
//
//	bool tagAlterPreserveFlag = afterExtendedHeader[8] >> 7, // what to do with frame if tag is unknown | 0 = preserve frame, 1 = discard frame
//			fileAlterPreserveFlag = afterExtendedHeader[8] >> 6, // what to do with frame if tag is unknown AND file is altered | 0 = preserve, 1 = discard
//			readOnlyFlag = afterExtendedHeader[8] >> 5, // usually true if it'll break something (ex: signatures)
//			remainingFrameStatusFlagsCleared = (afterExtendedHeader[8] & 0xF) == 0; // the usual - gotta have these cleared or "bad file"
//
//	bool groupingIdentityFlag = afterExtendedHeader[9] >> 7, // indicates if this frame is in group with others (????) | 0 = no group info, 1 = contains group info
//			compressionFlag = afterExtendedHeader[9] >> 4, // 0 = frame is not compressed, 1 = frame is compressed using zlib deflate method
//			encryptionFlag = afterExtendedHeader[9] >> 3, // 0 = not, 1 = is
//			frameUnsynchronisationFlag = afterExtendedHeader[9] >> 2, // indicates if unsynchronisation was applied to this frame | 0 = not, 1 = has
//			hasDataLengthFlag = afterExtendedHeader[9] >> 1; // indicates that a data length indicator is present | 0 = none, 1 = yes
//
//#ifdef SHOW_ADDITIONAL_DEBUG
//	cout << "Are any discard flags set: [" << tagAlterPreserveFlag << ", " << fileAlterPreserveFlag << "]" << endl;
//#endif
//
//	int tagEncoding = afterExtendedHeader[10] & 0x7F;
//
//#ifdef SHOW_ADDITIONAL_DEBUG
//	cout << "Encoding: " << translate_encoding(tagEncoding) << endl;
//#endif
//
//	cout << translate_tag(frameId) << ": " << string(afterExtendedHeader.begin() + 10, afterExtendedHeader.begin() + 10 + size) << endl;
//
//	input.close();
//}

int main() {
	//cout << "Parsing 'audio1.mp3':" << endl;
	//parse_file("mp3s/audio1.mp3");
	//cout << "=========================" << endl
	/*	 << endl
		 << "Parsing 'audio2.mp3':" << endl;
			parse_file("mp3s/audio2.mp3")*/
			;

	id3v2::ID3v2 audio1("mp3s/audio1.mp3");
	auto result = audio1.init();

	std::cout << "Init Result: " << result << std::endl;
	if (result != id3v2::OK) {
		return 1;
	}

	auto header = audio1.m_header;

	std::cout << "ID3v2 Headers:" << std::endl
			  << "===============" << std::endl;
	std::cout << "Version: ID3v2."
			  << header.major << "." << header.revision 
			  << std::endl;
	std::cout << "ID3v2 Tag Size: " << header.id3_size << " byte(s)" << std::endl;
	std::cout << checkbox(header.is_unsynchronisation) << " Unsynchronized" << std::endl;
	std::cout << checkbox(header.has_extended_header) << " Extended Header" << std::endl;
	std::cout << checkbox(header.has_experimental_indicator) << " Experimental Header" << std::endl;
	std::cout << checkbox(header.has_footer) << " Footer" << std::endl
			  << std::endl;

	if (header.has_extended_header) {
		auto extended = audio1.m_extended_header;
		std::cout << "Extended Header"
				  << "================" << std::endl;
		std::cout << "Size: " << extended.extended_header_size << " byte(s)" << std::endl
				  << std::endl;
	}

	for (auto &frame : audio1.m_frames) {
		std::cout << "Frame ID: " << frame.header.frame_id << std::endl;
		std::cout << "Data: " << frame.data << std::endl;
	}
	
	return 0;
}