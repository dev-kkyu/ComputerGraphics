#version 330 core

in vec3 FragPos;			//--- ��ġ��
in vec3 Normal;				//--- ���ؽ� ���̴����� ���� ��ְ�

out vec4 FragColor;			//--- ���� ��ü�� �� ����

uniform vec3 lightPos;		//--- ������ ��ġ
uniform vec3 lightColor;	//--- ������ ��
uniform vec3 objectColor;	//--- ��ü�� ��

uniform vec3 viewPos;		//--- ī�޶� ��ġ

void main()
{
	float ambientLight = 0.5;										//--- �ֺ� ���� ���: 0.0 �� ambientLight �� 1.
	vec3 ambient = ambientLight * lightColor;						//--- �ֺ� ���� ��

	vec3 normalVector = normalize (Normal);							//--- �븻���� ����ȭ�Ѵ�.
	vec3 lightDir = normalize (lightPos - FragPos);					//--- ǥ��� ������ ��ġ�� ������ ������ �����Ѵ�.

	float diffuseLight = max (dot (normalVector, lightDir), 0.0);	//--- N�� L�� ���� ������ ���� ���� (���� ���� ���� �� ���� �Ѵ�.)
	vec3 diffuse = diffuseLight * lightColor;						//--- ����ݻ����� = ����ݻ簪 * �������


	int shininess = 128;				//--- ���� ���
										//--- ��¦�̴� ���̶���Ʈ�� �����Ѵ�.
										//--- �����ڰ� ���� �Ի簢�� ���� ���� �ݻ簢 �αٿ� ��ġ�� ��� �Ի�� ���� ���θ� �ν��ϸ� ���̶���Ʈ�� �����.
										//--- �ſ� �ݻ� ������ ������ shininess (���� ���) ������ �߰�: shininess �� ������ ���� ������ ���̶���Ʈ�� �����ȴ�.

	vec3 viewDir = normalize (viewPos - FragPos);					//--- �������� ����
	vec3 reflectDir = reflect (-lightDir, normalVector);			//--- �ݻ� ����: reflect �Լ� - �Ի� ������ �ݻ� ���� ���

	float specularLight = max (dot (viewDir, reflectDir), 0.0);		//--- V�� R�� ���������� ���� ����: ���� ����
	specularLight = pow(specularLight, shininess);					//--- shininess ���� ���־� ���̶���Ʈ�� ������ش�.
	vec3 specular = specularLight * lightColor;						//--- �ſ� �ݻ� ����: �ſ�ݻ簪 * �������


	vec3 result = (ambient + diffuse + specular) * objectColor;		//--- ���� ���� ������ �ȼ� ����: (�ֺ����� + ����ݻ����� + �ſ�ݻ�����) * ��ü ����


	FragColor = vec4 (result, 1.0);									//--- �ȼ� ���� ���
}