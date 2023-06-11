#include "Model.h"
#include <iostream>

Model::Model(const char* filepath, const char* mtlpath)
{
	std::string Obj_path = filepath;
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = mtlpath;

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(Obj_path, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}

	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader: " << reader.Warning();
	}



	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	m_materials = reader.GetMaterials();


	for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);


			size_t face_idx = f;
			int mat_id = shapes[s].mesh.material_ids[f];
			for (size_t v = 0; v < fv; v++) {


				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

				m_meshes[mat_id].vertices.push_back(vx);
				m_meshes[mat_id].vertices.push_back(vy);
				m_meshes[mat_id].vertices.push_back(vz);

				if (idx.normal_index >= 0) {
					tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
					tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
					tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];

					m_meshes[mat_id].normals.push_back(nx);
					m_meshes[mat_id].normals.push_back(ny);
					m_meshes[mat_id].normals.push_back(nz);
				}

				if (idx.texcoord_index >= 0) {
					tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

					m_meshes[mat_id].texcoords.push_back(tx);
					m_meshes[mat_id].texcoords.push_back(1.0f - ty);
				}
			}
			index_offset += fv;

			//shapes[s].mesh.material_ids[f];
		}

	}

	for (size_t m = 0; m < m_materials.size(); m++) {
		auto& material = m_materials[m];
		//tinyobj::material_t;

		if (material.ambient_texname != "") {
			std::string tex_path = mtlpath + material.ambient_texname;
			ResourceManager::LoadTexture(tex_path.c_str(), true, material.ambient_texname);
		}
		if (material.diffuse_texname != "") {
			std::string tex_path = mtlpath + material.diffuse_texname;
			ResourceManager::LoadTexture(tex_path.c_str(), true, material.diffuse_texname);
		}
		if (material.specular_texname != "") {
			std::string tex_path = mtlpath + material.specular_texname;
			ResourceManager::LoadTexture(tex_path.c_str(), true, material.specular_texname);
		}
		if (material.specular_highlight_texname != "") {
			std::string tex_path = mtlpath + material.specular_highlight_texname;
			ResourceManager::LoadTexture(tex_path.c_str(), true, material.specular_highlight_texname);
		}
		if (material.bump_texname != "") {
			std::string tex_path = mtlpath + material.bump_texname;
			ResourceManager::LoadTexture(tex_path.c_str(), true, material.bump_texname);
		}
		if (material.displacement_texname != "") {
			std::string tex_path = mtlpath + material.displacement_texname;
			ResourceManager::LoadTexture(tex_path.c_str(), true, material.displacement_texname);
		}
		if (material.alpha_texname != "") {
			std::string tex_path = mtlpath + material.alpha_texname;
			ResourceManager::LoadTexture(tex_path.c_str(), true, material.alpha_texname);
		}
		if (material.reflection_texname != "") {
			std::string tex_path = mtlpath + material.reflection_texname;
			ResourceManager::LoadTexture(tex_path.c_str(), true, material.reflection_texname);
		}
	}
}

void Model::Draw(const char* shader, glm::mat4 modelmatrix)
{
	auto s = ResourceManager::GetShader(shader);
	s.SetMatrix4("M", modelmatrix);
	for (auto& [material_id, drawobject] : m_meshes) {

		float* vertices = (float*)drawobject.vertices.data();
		float* normals = (float*)drawobject.normals.data();
		float* texCoords = (float*)drawobject.texcoords.data();
		if (material_id >= 0) {
			auto& material = m_materials[material_id];
			if (ResourceManager::IsTextureLoaded(material.diffuse_texname)) {
				s.SetInteger("diffuseTexture", 1);
				glActiveTexture(GL_TEXTURE1);
				ResourceManager::GetTexture(material.diffuse_texname).Bind();
			}
			if (ResourceManager::IsTextureLoaded(material.ambient_texname)) {
				s.SetInteger("ambientTexture", 0);
				glActiveTexture(GL_TEXTURE0);
				ResourceManager::GetTexture(material.ambient_texname).Bind();
			}
			if (ResourceManager::IsTextureLoaded(material.specular_texname)) {
				s.SetInteger("specularTexture", 2);
				glActiveTexture(GL_TEXTURE2);
				ResourceManager::GetTexture(material.specular_texname).Bind();

			}
			if (ResourceManager::IsTextureLoaded(material.specular_highlight_texname)) {
				s.SetInteger("specular_highlightTexture", 3);
				glActiveTexture(GL_TEXTURE3);
				ResourceManager::GetTexture(material.specular_highlight_texname).Bind();
			}
			if (ResourceManager::IsTextureLoaded(material.bump_texname)) {
				s.SetInteger("bumbTexture", 4);
				glActiveTexture(GL_TEXTURE4);
				ResourceManager::GetTexture(material.bump_texname).Bind();
			}
			if (ResourceManager::IsTextureLoaded(material.displacement_texname)) {
				s.SetInteger("displacementTexture", 5);
				glActiveTexture(GL_TEXTURE5);
				ResourceManager::GetTexture(material.displacement_texname).Bind();
			}
			if (ResourceManager::IsTextureLoaded(material.alpha_texname)) {
				s.SetInteger("alphaTexture", 6);
				glActiveTexture(GL_TEXTURE6);
				ResourceManager::GetTexture(material.alpha_texname).Bind();
			}
			if (ResourceManager::IsTextureLoaded(material.reflection_texname)) {
				s.SetInteger("reflectionTexture", 7);
				glActiveTexture(GL_TEXTURE7);
				ResourceManager::GetTexture(material.reflection_texname).Bind();
			}

			s.SetVector3f("diffuseColor", material.diffuse[0], material.diffuse[1], material.diffuse[2]);
			s.SetVector3f("ambientColor", material.ambient[0], material.ambient[1], material.ambient[2]);
			s.SetVector3f("emissionColor", material.emission[0], material.emission[1], material.emission[2]);
			s.SetVector3f("specularColor", material.specular[0], material.specular[1], material.specular[2]);
			s.SetVector3f("transmittanceColor", material.transmittance[0], material.transmittance[1], material.transmittance[2]);

			
			s.SetFloat("dissolve", material.dissolve);
			s.SetFloat("ior", material.ior);
			s.SetFloat("shininess", material.shininess);
		}

		s.EnableAttribPointerf("vertex", 3, vertices);
		s.EnableAttribPointerf("normal", 3, normals);
		s.EnableAttribPointerf("texCoord0", 2, texCoords);

		glDrawArrays(GL_TRIANGLES, 0, drawobject.vertices.size() / 3); //Narysuj obiekt

		s.DisableAttribPointerf("vertex");
		s.DisableAttribPointerf("normal");
		s.DisableAttribPointerf("texCoord0");
	}
	
}

Model::~Model()
{

}
