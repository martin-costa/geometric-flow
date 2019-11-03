# geometric-flow
A simulation of 2D-curve shortening flow. Each point on the curve moves in the direction of its curvature proportional to the curvature at that point. For each point on the curve, the curvature is calculated by finding the radius of the circle that goes through that point and the two adjacent points, since this will approximately be the radius of the osculating circle, the reciprocal of the radius gives a good approximation of the curvature.
