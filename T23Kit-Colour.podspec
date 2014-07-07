Pod::Spec.new do |s|
  s.name         = "T23Kit-Colour"
  s.version      = "0.0.1"
  s.summary      = "UIColor Category with various colorspace transformations and ∆-E methods."
  s.description  = <<-DESC
                  UIColor category with support for non-standard colorspace conversions from RGB, neighbour color generation methods, and color distance methods.
                   DESC
  s.homepage     = "https://github.com/thirteen23/T23Kit-Colour"
  s.license      = { :type => "MIT", :file => "LICENSE" }
  s.author             = { "Michael Van Milligan" => "michael.vanmilligan@thirteen23.com" }
  s.platform     = :ios, "5.0"
  s.source       = { :git => "https://github.com/thirteen23/T23Kit-Colour.git", :tag => "0.0.1" }
  s.source_files  = "T23Kit-Colour"
  s.public_header_files = "T23Kit-Colour/*.h"
  s.frameworks = "UIKit"
  s.requires_arc = false
end