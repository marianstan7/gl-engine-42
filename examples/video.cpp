//
// video.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar 16 10:56:19 2012 gael jochaud-du-plessix
// Last update Thu Apr 12 19:36:59 2012 loick michard
//

#include <iomanip>
#include <iostream>
#include <sstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "video.hpp"

using namespace video;

sf::Clock video::videoTimer;
int video::currentVideoFrame = 0;
sf::Texture video::videoTexture;

void video::saveImage(sf::Window &app, int framerate)
{
  if (!app.isOpen())
    return ;
  sf::Vector2u size = app.getSize();
  if (videoTexture.getSize().y != size.y)
    videoTexture.create(size.x, size.y);

  if (videoTimer.getElapsedTime().asMilliseconds() > 1000/framerate)
    {
      videoTimer.restart();

      std::ostringstream videoFramePath;

      videoFramePath << "/tmp/gl-engine-"
		     << std::setw(5)
		     << std::setfill('0')
		     << currentVideoFrame
		     << ".jpg";
      videoTexture.update(app);
      sf::Image videoImage = videoTexture.copyToImage();
      videoImage.saveToFile(videoFramePath.str());
      currentVideoFrame++;
    }
}

void video::save(std::string name, int framerate)
{
  std::cout << "Saving video..." << std::endl;
  std::ostringstream videoFilename;
  std::ostringstream videoCommand;
  videoFilename << name << "_" << std::time(NULL) << ".mp4";
  videoCommand << "ffmpeg -r " << framerate
	       << " -b 20000kb -i /tmp/gl-engine-%05d.jpg "
	       << videoFilename.str()
	       << " 2> /tmp/gl-engine-video.log";
  system(videoCommand.str().c_str());
  std::cout << "Video saved to " << videoFilename.str() << std::endl;
  system("rm /tmp/gl-engine-*.jpg");
}
