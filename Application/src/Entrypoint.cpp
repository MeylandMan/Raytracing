#include <fstream>

#include "Core/Application.h"

int main(int argc, char** argv) {

	Application* app = new Application(800, 600, "RayTracing in one Weekend");
	app->Run();

	delete app;
	return 0;

}
/*

int main() {
	// Image
	int image_width = 256;
	int image_height = 256;

	// Création du fichier PPM
	std::ofstream image_file("../image.ppm");

	// Vérification si le fichier s'est bien ouvert
	if (!image_file) {
		std::cerr << "Erreur lors de la création du fichier image.ppm\n";
		return 1;
	}

	// En-tête PPM
	image_file << "P3\n" << image_width << " " << image_height << "\n255\n";

	// Génération des pixels
	for (unsigned int j = 0; j < image_height; j++) {  // Correction de l'inversion hauteur/largeur
		std::clog << "\rScanlines remaining: " << (image_width - j) << " " << std::flush;
		for (unsigned int i = 0; i < image_width; i++) {
			auto r = double(i) / (image_width - 1);
			auto g = double(j) / (image_height - 1);
			auto b = 0.0;

			int ir = int(255.999 * r);
			int ig = int(255.999 * g);
			int ib = int(255.999 * b);

			image_file << ir << " " << ig << " " << ib << " ";
		}
		image_file << "\n";
	}


	// Fermeture du fichier
	image_file.close();
	std::clog << "\rDone.                  \n";

	return 0;
}
*/


