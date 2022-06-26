#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include "Shape.hpp"
#include "Sphere.hpp"
#include "Box.hpp"
#include <iostream>

# include <glm/glm.hpp>
# include <glm/gtx/intersect.hpp>

int main(int argc, char *argv[])
{
  return Catch::Session().run(argc, argv);
}

TEST_CASE("Box_area","[Box]") {
	Box box_1{ glm::vec3{1.0f,1.0f,1.0f}, glm::vec3{2.0f,2.0f,2.0f} };
	Box box_2{ glm::vec3{0.0f,0.0f,0.0f}, glm::vec3{3.0f,2.0f,2.5f} };
	Box box_3{ glm::vec3{0.0f,0.0f,0.0f}, glm::vec3{-3.0f,-3.0f,-3.0f} };
	Box box_4{ glm::vec3{0.0f,0.0f,0.0f}, glm::vec3{0.0f,0.0f,0.0f} };



	REQUIRE(box_1.area() == 6.0f);
	REQUIRE(box_1.volume() == 1.0f);

	REQUIRE(box_2.area() == 37.0f);
	REQUIRE(box_2.volume() == 15.0f);

	REQUIRE(box_3.area() == 54.0f);
	REQUIRE(box_3.volume() == 27.0f);

	REQUIRE(box_4.area() == 0.0f);
	REQUIRE(box_4.volume() == 0.0f);
}

TEST_CASE("Output", "[Shape]") {
	Sphere sp_1{ "Sphere", Color{1.0f,1.0f,1.0f}, glm::vec3{0.2f,-0.3,9.5f},2.0f };
	Sphere sp_2{ glm::vec3{ 3.5f,-3.5f,0.0f },-2.0f };

	Box b_1{ "Box", {0.0f,0.2f,3.1f}, {0.0f,1.2,0.01f}, {0.1f,-1.3f,1.0f} };
	Box b_2{ {-3.0f,9.2f,9.1f}, {4.1f,-2.4f,0.3f} };
	std::cout << "Sphere:\n" << sp_1 << std::endl;
	std::cout << "Sphere:\n" << sp_1 << "\n" << sp_2 << "\n"<< "Box:\n" << b_1 << "\n" << b_2 << std::endl;
}

TEST_CASE("Sphere_area","[Sphere]") {
	Sphere sp_1{ glm::vec3{},2.0f };
	Sphere sp_2{ glm::vec3{},-2.0f };
	Sphere sp_3{ glm::vec3{},0.0f };
	Sphere sp_4{ glm::vec3{},-1.2f };

	REQUIRE(sp_1.area() == Approx(50.26548f));
	REQUIRE(sp_1.volume() == Approx(33.51032f));

	REQUIRE(sp_2.area() == Approx(50.26548f));
	REQUIRE(sp_2.volume() == Approx(33.51032f));

	REQUIRE(sp_3.area() == 0);
	REQUIRE(sp_3.volume() == 0);

	REQUIRE(sp_4.area() == Approx(18.09558f));
	REQUIRE(sp_4.volume() == Approx(7.23823f));
}

TEST_CASE("intersect_ray_sphere", " [Sphere] ")
{
	// Ray
	glm::vec3 ray_origin{ 0.0f , 0.0f , 0.0f };
	// ray direction has to be normalized !
	// you can use :
	// v = glm :: normalize ( some_vector )
	glm::vec3 ray_direction{ 0.0f , 0.0f , 1.0f };
	// Sphere
	glm::vec3 sphere_center{ 0.0f ,0.0f , 5.0f };
	float sphere_radius{ 1.0f };
	float distance = 0.0f;
	auto result = glm::intersectRaySphere(
		ray_origin, ray_direction,
		sphere_center,
		sphere_radius * sphere_radius, 
		distance);
	REQUIRE(distance == Approx(4.0f));

	Ray ra{ glm::vec3{ 0.0f,0.0f,0.0f }, glm::vec3{ 0.0f,0.0f,0.0f } };
	Ray rb{ glm::vec3{ 0.1f,0.3f,3.0f },glm::vec3{ -1.1f, 1.3f, -3.2f } };
	Ray rc{};

	Sphere a{ glm::vec3{ 1.0f,0.4f,3.2f }, 3.0f };
	Sphere b{ glm::vec3{ 0.1f,0.3f,3.0f }, 3.0f };
	Sphere c{ glm::vec3{ 0.2f,0.0f,2.0f },0.0f };
	Sphere d{ glm::vec3{ 0.2f,0.0f,-2.0f }, 0.0f };

	REQUIRE(a.intersect(ra).hit == false);

	REQUIRE(a.intersect(rb).hit == true);
	REQUIRE(a.intersect(rb).t == Approx(2.46912f));

	REQUIRE(a.intersect(rc).hit == false);
	//
	REQUIRE(b.intersect(ra).hit == false);
	
	REQUIRE(b.intersect(rb).hit == true);
	REQUIRE(b.intersect(rb).t == Approx(3.0f));

	REQUIRE(b.intersect(rc).hit == false);
	//
	REQUIRE(c.intersect(ra).hit == false);
	REQUIRE(c.intersect(rb).hit == false);
	REQUIRE(c.intersect(rc).hit == false);
	//
	REQUIRE(d.intersect(ra).hit == false);
	REQUIRE(d.intersect(rb).hit == false);
	REQUIRE(d.intersect(rc).hit == false);


	
}
TEST_CASE("Destructor", " [Destructor] "){
	Color red{ 255 , 0 , 0 };
	glm::vec3 position{ 0.0f , 0.0f , 0.0f };
	Sphere* s1 = new Sphere{" sphere0 " ,red, position , 1.2f };
	Shape* s2 = new Sphere{ "sphere1" , red , position , 1.2f };
	s1->print(std::cout);
	s2->print(std::cout);
	delete s1;
	delete s2;
}