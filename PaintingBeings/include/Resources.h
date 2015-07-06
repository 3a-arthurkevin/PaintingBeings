#pragma once
#include "cinder/CinderResources.h"

// Ficher donnant le chemin des ressources --> ici les fichiers .glsl
// Voir le fichier Ressources.rc si on veut ajouter d'autres ressources

//#define RES_MY_RES			CINDER_RESOURCE( ../resources/, image_name.png, 128, IMAGE )
#define RES_SHADER_FRAG		CINDER_RESOURCE( ../resources/, frag.glsl, 128, GLSL )
#define RES_SHADER_VERT		CINDER_RESOURCE( ../resources/, vert.glsl, 129, GLSL )



