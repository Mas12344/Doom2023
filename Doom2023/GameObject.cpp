#include "GameObject.h"

GameObject::GameObject(std::shared_ptr<Model> model) : m_Model(model), m_ModelMatrix(glm::mat4(1)) {}

std::shared_ptr<Model> GameObject::GetModel()
{
    return m_Model;
}

void GameObject::SetModel(std::shared_ptr<Model> model)
{
    m_Model = model;
}

glm::mat4 GameObject::ApplyTransform(Transformation t)
{
    TranformType tt = t.tt;
    switch (tt)
    {
    case TranformType::Translate:
        m_ModelMatrix = glm::translate(m_ModelMatrix, t.val);
        break;
    case TranformType::Scale:
        m_ModelMatrix = glm::scale(m_ModelMatrix, t.val);
        break;
    case TranformType::Rotate:
        m_ModelMatrix = glm::rotate(m_ModelMatrix, t.angle, t.axis);
        break;
    default:
        break;
    }
    return m_ModelMatrix;
}
