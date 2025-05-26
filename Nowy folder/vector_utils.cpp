#include "vector_utils.h"

sf::Vector2f max(const sf::Vector2f& a, float b) {
	return sf::Vector2f(std::max(a.x, b), std::max(a.y, b));
}

float min(float a, float b) {
	return std::min(a, b);
}

float length(const sf::Vector2f v) {
	return std::sqrt(v.x * v.x + v.y * v.y);
}
