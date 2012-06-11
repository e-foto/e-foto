TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS =	br.mil.eb.synapseLauncher \
			br.mil.eb.synapse.view \
			br.mil.eb.synapseGenerator \
			br.mil.eb.synapseGenerator.view \
			br.uerj.eng.efoto.teste.app \
			br.uerj.eng.efoto.engine \
			br.uerj.eng.efoto.viewer \
			br.uerj.eng.efoto.app

#Examples
SUBDIRS +=	br.mil.eb.example.divide \
			br.mil.eb.example.calculator \
			br.mil.eb.example.calculator.view
