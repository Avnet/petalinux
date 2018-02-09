#!/bin/sh

# Add any custom code to be run at startup here

echo Create symbolic links for libboost libraries

cd /usr/lib
BOOST_REV="1.61.0"
       
ln -s libboost_atomic.so.${BOOST_REV}		libboost_atomic.so
ln -s libboost_chrono.so.${BOOST_REV}		libboost_chrono.so
ln -s libboost_context.so.${BOOST_REV}		libboost_context.so
ln -s libboost_date_time.so.${BOOST_REV} 	libboost_date_time.so
ln -s libboost_filesystem.so.${BOOST_REV} 	libboost_filesystem.so
ln -s libboost_graph.so.${BOOST_REV} 		libboost_graph.so
ln -s libboost_iostreams.so.${BOOST_REV} 	libboost_iostreams.so
ln -s libboost_locale.so.${BOOST_REV} 		libboost_locale.so
ln -s libboost_math_c99.so.${BOOST_REV} 	libboost_math_c99.so
ln -s libboost_math_c99f.so.${BOOST_REV} 	libboost_math_c99f.so
ln -s libboost_math_c99l.so.${BOOST_REV} 	libboost_math_c99l.so
ln -s libboost_math_tr1.so.${BOOST_REV} 	libboost_math_tr1.so
ln -s libboost_math_tr1f.so.${BOOST_REV} 	libboost_math_tr1f.so
ln -s libboost_math_tr1l.so.${BOOST_REV} 	libboost_math_tr1l.so
ln -s libboost_prj_exec_monitor.so.${BOOST_REV} libboost_prj_exec_monitor.so
ln -s libboost_program_options.so.${BOOST_REV} 	libboost_program_options.so
ln -s libboost_random.so.${BOOST_REV} 		libboost_random.so
ln -s libboost_regex.so.${BOOST_REV} 		libboost_regex.so
ln -s libboost_serialization.so.${BOOST_REV} 	libboost_serialization.so
ln -s libboost_signals.so.${BOOST_REV} 		libboost_signals.so
ln -s libboost_system.so.${BOOST_REV} 		libboost_system.so
#ln -s libboost_test_exec_monitor.so.${BOOST_REV} libboost_test_exec_monitor.so
ln -s libboost_thread.so.${BOOST_REV} 		libboost_thread.so
ln -s libboost_timer.so.${BOOST_REV} 		libboost_timer.so
ln -s libboost_unit_test_framework.so.${BOOST_REV} libboost_unit_test_framework.so
ln -s libboost_wave.so.${BOOST_REV} 		libboost_wave.so
ln -s libboost_wserialization.so.${BOOST_REV} 	libboost_wserialization.so

echo DONE!
