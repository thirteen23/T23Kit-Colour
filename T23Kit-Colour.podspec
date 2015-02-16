Pod::Spec.new do |s|
  s.name         = "T23Kit-Colour"
  s.version      = "0.0.9"
  s.summary      = "UIColor Category with various colourspace transformations and ∆-E methods."
  s.description  = <<-DESC
                  UIColor category with support for non-standard colorspace conversions from RGB, neighbour colour generation methods, and colour distance methods.
                   DESC
  s.homepage     = "https://github.com/thirteen23/T23Kit-Colour"
  s.license      = { :type => "MIT", :file => "LICENSE" }
  s.author             = { "Michael Van Milligan" => "michael.vanmilligan@thirteen23.com" }
  s.platform     = :ios, "7.0"
  s.source       = { :git => "https://github.com/thirteen23/T23Kit-Colour.git", :tag => "0.0.9" }
  s.source_files  = "T23Kit-Colour/ObjC/{Categories,libcolourspaces}/*.{h,m,c}"
  s.public_header_files = "T23Kit-Colour/ObjC/{Categories,libcolourspaces}/*.h"
  s.frameworks = "UIKit"
  s.requires_arc = false
end
