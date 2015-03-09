// TagLibEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <fileref.h>
#include <tag.h>
#include <tpropertymap.h>
#include <iomanip>

#pragma comment(lib, "tag.lib")

using namespace std;

TagLib::String formatSeconds(int seconds)
{
	char secondsString[3];
	sprintf_s(secondsString, "%02i", seconds);
	return secondsString;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 2)
	{
		cout << "usage : TagLibEx filename" << endl;
		return 0;
	}

	TagLib::FileRef f(argv[1]);

	if (!f.isNull() && f.tag()) {

		TagLib::Tag *tag = f.tag();

		cout << "-- TAG (basic) --" << endl;
		cout << "title   - \"" << tag->title() << "\"" << endl;
		cout << "artist  - \"" << tag->artist() << "\"" << endl;
		cout << "album   - \"" << tag->album() << "\"" << endl;
		cout << "year    - \"" << tag->year() << "\"" << endl;
		cout << "comment - \"" << tag->comment() << "\"" << endl;
		cout << "track   - \"" << tag->track() << "\"" << endl;
		cout << "genre   - \"" << tag->genre() << "\"" << endl;

		TagLib::PropertyMap tags = f.file()->properties();

		unsigned int longest = 0;
		for (TagLib::PropertyMap::ConstIterator i = tags.begin(); i != tags.end(); ++i) {
			if (i->first.size() > longest) {
				longest = i->first.size();
			}
		}

		cout << "-- TAG (properties) --" << endl;
		for (TagLib::PropertyMap::ConstIterator i = tags.begin(); i != tags.end(); ++i) {
			for (TagLib::StringList::ConstIterator j = i->second.begin(); j != i->second.end(); ++j) {
				cout << left << std::setw(longest) << i->first << " - " << '"' << *j << '"' << endl;
			}
		}

	}

	if (!f.isNull() && f.audioProperties()) {

		TagLib::AudioProperties *properties = f.audioProperties();

		int seconds = properties->length() % 60;
		int minutes = (properties->length() - seconds) / 60;

		cout << "-- AUDIO --" << endl;
		cout << "bitrate     - " << properties->bitrate() << endl;
		cout << "sample rate - " << properties->sampleRate() << endl;
		cout << "channels    - " << properties->channels() << endl;
		cout << "length      - " << minutes << ":" << formatSeconds(seconds) << endl;
	}

	return 0;
}

