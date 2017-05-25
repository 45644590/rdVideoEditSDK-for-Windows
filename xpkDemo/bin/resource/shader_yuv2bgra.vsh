uniform mat4 matViewProjection;		//������ľ���(��ת/ƽ��/����/͸��ͶӰ)
attribute vec4 vertexPosition;
attribute vec2 inputTextureCoordinate;

varying vec2 textureCoordinate;


void main()
{
	gl_Position = matViewProjection*vertexPosition;
	textureCoordinate = inputTextureCoordinate;

	
}
 