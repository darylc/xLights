#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <string>
#include <list>
#include <mutex>

#include "sequencer/mpg123.h"
#include "vamp-hostsdk/PluginLoader.h"
#include "JobPool.h"

class AudioManager;
class xLightsXmlFile;

class AudioScanJob : Job
{
private:
	AudioManager* _audio;
	std::string _status;
public:
	AudioScanJob(AudioManager* audio);
	virtual ~AudioScanJob() {};
	virtual void Process();
	virtual std::string GetStatus() { return _status; }
};

class xLightsVamp
{
	Vamp::HostExt::PluginLoader *_loader;
	std::map<std::string, Vamp::Plugin *> _plugins;
	std::vector<Vamp::Plugin *> _loadedPlugins;
	std::map<std::string, Vamp::Plugin *> _allplugins;
	std::vector<Vamp::Plugin *> _allloadedPlugins;

public:

	struct PluginDetails
	{
	public:
		std::string Name;
		std::string Description;
	};

	xLightsVamp();
	~xLightsVamp();
	std::string ProcessPlugin(AudioManager* paudio, std::string& plugin);
	void ProcessFeatures(Vamp::Plugin::FeatureList &feature, std::vector<int> &starts, std::vector<int> &ends, std::vector<std::string> &labels);
	std::list<std::string> GetAvailablePlugins(AudioManager* paudio);
	std::list<std::string> GetAllAvailablePlugins(AudioManager* paudio);
	Vamp::Plugin* GetPlugin(std::string name);
};

typedef enum FRAMEDATATYPE {
	FRAMEDATA_HIGH,
	FRAMEDATA_LOW,
	FRAMEDATA_SPREAD,
	FRAMEDATA_VU,
	FRAMEDATA_ISTIMINGMARK
} FRAMEDATATYPE;

class AudioManager
{
	JobPool _jobPool;
	Job* _job;
	std::mutex _mutex;
	std::vector<std::vector<std::list<float>>> _frameData;
	std::string _audio_file;
	xLightsVamp _vamp;
	long _rate;
	int _channels;
	int _trackSize;
	int _encoding;
	int _bits;
	int _extra;
	std::string _resultMessage;
	int _state;
	float *_data[2]; // audio data
	std::string _title;
	std::string _artist;
	std::string _album;
	int _intervalMS;
	int _lengthMS;
	bool _isCBR;
	bool _frameDataPrepared;
	xLightsXmlFile* _xml_file;
	float _bigmax;
	float _bigspread;
	float _bigmin;
	float _bigspectogrammax;

	int CalcTrackSize(mpg123_handle *phm, int bits, int channels);
	int CalcLengthMS();
	void SplitTrackDataAndNormalize(signed short* trackData, int trackSize, float* leftData, float* rightData);
	void NormalizeMonoTrackData(signed short* trackData, int trackSize, float* leftData);
	void LoadTrackData(mpg123_handle *phm, char* data, int maxSize);
	int OpenMediaFile();
	void ExtractMP3Tags(mpg123_handle *phm);
	bool CheckCBR();
	void PrepareFrameData();
	int decodebitrateindex(int bitrateindex, int version, int layertype);
	int decodesamplerateindex(int samplerateindex, int version);
	int decodesideinfosize(int version, int mono);
	std::list<float> ProcessFeatures(Vamp::Plugin::FeatureList &feature, float& max);

public:

	xLightsVamp* GetVamp() { return &_vamp; };
	AudioManager(std::string audio_file, xLightsXmlFile* xml_file, int step, int block);
	~AudioManager();
	int GetTrackSize() { return _trackSize; };
	long GetRate() { return _rate; };
	int GetChannels() { return _channels; };
	int GetState() { return _state; };
	std::string GetResultMessage() { return _resultMessage; };
	std::string GetVampTiming(std::string plugin);
	std::string Title() { return _title; };
	std::string Artist() { return _artist; };
	std::string Album() { return _album; };
	std::string FileName() { return _audio_file; };
	int LengthMS() { return _lengthMS; };
	float GetRightData(int offset);
	float GetLeftData(int offset);
	float* GetRightDataPtr(int offset);
	float* GetLeftDataPtr(int offset);
	void SetStepBlock(int step, int block);
	void SetFrameInterval(int intervalMS);
	int GetFrameInterval() { return _intervalMS; }
	std::list<float>* GetFrameData(int frame, FRAMEDATATYPE fdt, std::string timing);
	bool IsCBR() { return _isCBR; };
	void DoPrepareFrameData();
};

#endif