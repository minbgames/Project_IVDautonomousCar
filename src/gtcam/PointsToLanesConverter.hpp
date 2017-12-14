#pragma once

#include <vector>
#include <iostream>
#include "LaneModels.hpp"
#include "RANSAC.hpp"

namespace vision
{

class PointsToLanesConverter
{
public:
	int x_left = 0, x_right = 0;
	std::vector<lane_model::Parabola> Convert_left(std::vector<cv::Point2f> points)
	{
		std::vector<lane_model::Parabola> model;

		std::vector<cv::Point2f> left_points;

		float average_left = 0;
		float average = 0;
		int size = 0;
		bool standard = false;
		bool initiate = false;
		float x = 0, y = 0;
		float x_old = 0, x_lane = 0;
		float y_old = 0;

		int pointSize = points.size() - 1;
		for (int k = pointSize; k > 0; k--)
		{
			y_old = y;
			x = points[k].x;
			y = points[k].y;

			if(standard)
			{
				if (initiate)
				{
					if (y == y_old)
					{
						if (std::abs(x_old - x) < 5)
						{
							left_points.emplace_back(x, y);
							average = average_left * size + x;
							size++;
						}
					}
					else if ((y_old - y) == 1)
					{
						x_old = average;
						size = 0;
						average_left = 0;
						if (std::abs(x_old - x) < 5)
						{
							left_points.emplace_back(x, y);
							average = average_left * size + x;
							size++;
						}
					}
					else
					{
						if (std::abs(x_old - x) < 15)
						{
							x_old = x;
							left_points.emplace_back(x, y);
						}
					}
				}
			}

			if ((initiate == false) && (y != y_old)) initiate = true;
			if (initiate == false)
			{
				if (std::abs(average_left - 280) < std::abs(x - 280))
				{
					x_old = x;
					left_points.clear();
					left_points.emplace_back(x, y);
					x_left = x_old;
				}
			}

			if (standard == false && (x < 340))
			{
				standard = true;
				x_old = x;
				left_points.emplace_back(x, y);
				x_left = x_old;
			}

			//y_old = y;
		}

		//left point check
		/*
		for (int k = 0; k < left_points.size(); k++)
		{
			float x = left_points[k].x;
			float y = left_points[k].y;

			std::cout << x << "     " << y << std::endl;
		}
		*/
		if (left_points.size() > 50)
		{
			auto parabola_left = RANSAC_Parabola(RANSAC_ITERATIONS, RANSAC_MODEL_SIZE, static_cast<int>(RANSAC_INLINERS * left_points.size()), RANSAC_ERROR_THRESHOLD, left_points);
			model.emplace_back(parabola_left);
		}
		else {
			model.clear();
		}

		return model;
	}

	std::vector<lane_model::Parabola> Convert_right(std::vector<cv::Point2f> points)
	{
		std::vector<lane_model::Parabola> model;

		std::vector<cv::Point2f> right_points;

		float average_right = 0;
		float average = 0;
		int size = 0;
		bool standard = false;
		bool initiate = false;
		float x = 0, y = 0;
		float x_old = 0, x_lane =0;
		float y_old = 0;

		int pointSize = points.size() - 1;
		for (int k = pointSize; k > 0; k--)
		{
			y_old = y;
			x = points[k].x;
			y = points[k].y;

			if(standard)
			{
				if (initiate)
				{
					if (y == y_old)
					{
						if (std::abs(x_old - x) < 5)
						{
							right_points.emplace_back(x, y);
							average = average_right * size + x;
							size++;
						}
					}
					else if ((y_old - y) == 1)
					{
						x_old = average;
						size = 0;
						average_right = 0;
						if (std::abs(x_old - x) < 5)
						{
							right_points.emplace_back(x, y);
							average = average_right * size + x;
							size++;
						}
					}
					else
					{
						if (std::abs(x_old - x) < 15)
						{
							x_old = x;
							right_points.emplace_back(x, y);
						}
					}
				}
			}

			if ((initiate == false) && (y != y_old)) initiate = true;
			if (initiate == false)
			{
				if (std::abs(average_right - 400) < std::abs(x - 400))
				{
					x_old = x;
					right_points.clear();
					right_points.emplace_back(x, y);
					x_right = x_old;
				}
			}


			if (standard == false && (x > 340))
			{
				standard = true;
				x_old = x;
				right_points.emplace_back(x, y);
				x_right = x_old;
			}

			//y_old = y;
		}

		// right point check
		/*
		for (int k = 0; k < right_points.size(); k++)
		{
			float x = right_points[k].x;
			float y = right_points[k].y;

			std::cout << x << "     " << y << std::endl;
		}
		*/

		if (right_points.size() > 50)
		{
			auto parabola_right = RANSAC_Parabola(RANSAC_ITERATIONS, RANSAC_MODEL_SIZE, static_cast<int>(RANSAC_INLINERS * right_points.size()), RANSAC_ERROR_THRESHOLD, right_points);
			model.emplace_back(parabola_right);
		}
		else {
			model.clear();
		}

		return model;
	}

private:
	const int RANSAC_ITERATIONS = 100;
	const int RANSAC_MODEL_SIZE = 3;
	const int RANSAC_ERROR_THRESHOLD = 30;
	const double RANSAC_INLINERS = 0.55;
};

} // namespace vision
