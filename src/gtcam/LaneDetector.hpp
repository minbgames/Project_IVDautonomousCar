#pragma once
#include <opencv2/core/core.hpp>
#include <vector>

#include "LaneModels.hpp"

namespace vision
{

	struct CurrentLaneModel
	{
		CurrentLaneModel() : valid(false) {}

		lane_model::Parabola left_;
		lane_model::Parabola right_;
		lane_model::Parabola center;

		bool valid = false;
	};

	struct RoadModel
	{
		RoadModel() {}

		std::vector<lane_model::Parabola> lanes_left;
		std::vector<lane_model::Parabola> lanes_right;
		std::vector<lane_model::Parabola> current_lane_left;
		std::vector<lane_model::Parabola> current_lane_right;
		CurrentLaneModel current_lane_model_;

		cv::Mat invPerspTransform;
	};

	class LaneDetector
	{
	public:
		LaneDetector();
		RoadModel DetectLane(cv::Mat& inputFrame);
		int Inputsign(int input);
		int SetSteering(int steer);
		double stop_line_distance();
		double FindingCenter();
		//double FindingCentre();
		double FindingCurvature();
		double Parkgoal();
		RoadModel build();

	private:
		cv::Mat FindPixelsThatMayBelongToLane(const cv::cuda::GpuMat& input);

		cv::Mat DownsampleImageByHalf(const cv::Mat& input);

		cv::Mat perspectiveTransform;
		cv::Mat invPerspectiveTransform;

		int steering = 0;
	};


}   // namespace vision
