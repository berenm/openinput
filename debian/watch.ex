# Example watch control file for uscan
# Rename this file to "watch" and then you can run the "uscan" command
# to check for upstream updates and more.
# See uscan(1) for format

# Compulsory line, this is a version 3 file
version=3

# Uncomment to examine a Webpage 
# <Webpage URL> <string match>
#http://www.example.com/downloads.php libopeninput-(.*)\.tar\.gz

# Uncomment to examine a Webserver directory
#http://www.example.com/pub/libopeninput-(.*)\.tar\.gz

# Uncommment to examine a FTP server
#ftp://ftp.example.com/pub/libopeninput-(.*)\.tar\.gz debian uupdate

# Uncomment to use Roland's hack for sourceforge based projects - YMMV!
#http://people.debian.org/~lolando/sfdlr.php?project=libopeninput libopeninput-([\d.]*).tar.gz


