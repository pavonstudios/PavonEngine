struct Trigger{
	bool collision = false;
	bool positive_x = false;
	bool negative_x = false;
	bool positive_y = false;
	bool negative_y = false;
};

struct SphereCollider{
	vec3 center;
	vec3 radius;
};

struct AABB{
	glm::vec3 m_vecMax;
	glm::vec3 m_vecMin;
};

struct Collider{
	bool collision = false;
	bool positive_x = false;
	bool negative_x = false;
	bool positive_y = false;
	bool negative_y = false;
	bool can_move_negative_x = true;
	bool can_move_positive_x = true;
	bool can_move_negative_y = true;
	bool can_move_positive_y = true;
};