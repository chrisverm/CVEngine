#ifndef FRAME_PARAMS_H
#define FRAME_PARAMS_H

namespace CVE
{
	namespace Graphics
	{
		enum FrameStage : unsigned char // uint8_t
		{
			GAME = 0,
			RENDER = 1,
			GPU = 2
		};

		struct FrameParams
		{
			double DeltaTime;
			double StartTime, EndTime;

			FrameStage CurrentStage;
			int Data;
		};
	}
}

#endif