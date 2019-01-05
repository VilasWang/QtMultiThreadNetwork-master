
TEMPLATE = subdirs
CONFIG -= ordered
SUBDIRS += qmultithreadnetwork test
qmultithreadnetwork.file = src/QMultiThreadNetwork.pro
test.depends = qmultithreadnetwork

OTHER_FILES += README.md
#EssentialDepends = 
#OptionalDepends =
