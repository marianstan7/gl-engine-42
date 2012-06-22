#!/usr/bin/php
<?php
$shaders = array(
		 'gle::ShaderSource::VertexShader' => 'Vertex.glsl',
		 'gle::ShaderSource::FragmentShader' => 'Fragment.glsl',
		 'gle::ShaderSource::CubeMapVertexShader' => 'CubeMapVertex.glsl',
		 'gle::ShaderSource::CubeMapFragmentShader' => 'CubeMapFragment.glsl'
		 );

$path = realpath(dirname(__FILE__));

ob_start();
?>
/**************************************/
/*        Shaders sources file        */
/* Generated with genShaderSource.php */
/**************************************/

#include <gle/opengl.h>
#include <ShaderSource.hpp>

<?php
$file = ob_get_clean();

foreach ($shaders as $var => $shader)
{
  $content = file_get_contents($path . '/' . $shader);
  $file .= 'const char* ' . $var . ' = ' . "\n";
  foreach (explode("\n", $content) as $line)
    $file .= '"' . str_replace('"', '\\"', $line) . '\n"' . "\n";
  $file .= ";\n\n";
}

file_put_contents($path . '/GenShaders.cpp', $file);