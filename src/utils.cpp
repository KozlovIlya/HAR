#include "utils.hpp"

#include <glm/glm.hpp>
#include <iostream>

std::vector<glm::vec2> HAR::Math::InflatePolygon(const std::vector<glm::vec2>& vertices, float radius) {
    std::vector<glm::vec2> inflatedVertices;
    int n = vertices.size();

    for (int i = 0; i < n; i++) {
        glm::vec2 edge = vertices[(i + 1) % n] - vertices[i];
        glm::vec2 normal(-edge.y, edge.x);
        normal = glm::normalize(normal);
        inflatedVertices.push_back(vertices[i] + normal * radius);
        inflatedVertices.push_back(vertices[(i + 1) % n] + normal * radius);
    }

    return inflatedVertices;
}

std::optional<glm::vec2> HAR::Math::getIntersection(
    const glm::vec2& sA, const glm::vec2& fA,
    const glm::vec2& sB, const glm::vec2& fB)
{
    glm::vec2 r = fA - sA; // Вектор отрезка AB
    glm::vec2 s = fB - sB; // Вектор отрезка CD
    glm::vec2 qp = sB - sA; // Вектор между начальными точками AB и CD
    float rxs = glm::cross(glm::vec3(r, 0.0f), glm::vec3(s, 0.0f)).z;
    float qpxr = glm::cross(glm::vec3(qp, 0.0f), glm::vec3(r, 0.0f)).z;

    if (abs(rxs) < 1e-8) {
        return {}; // Отрезки параллельны или лежат на одной прямой
    }

    float t = glm::cross(glm::vec3(qp, 0.0f), glm::vec3(s, 0.0f)).z / rxs;
    float u = qpxr / rxs;
    if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) {
        return sA + t * r; // Точка пересечения находится на обоих отрезках
    }

    return {}; // Отрезки не пересекаются
}

