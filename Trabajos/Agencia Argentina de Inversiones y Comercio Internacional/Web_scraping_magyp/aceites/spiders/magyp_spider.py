import scrapy

class MagypSpiderSpider(scrapy.Spider):
    name = "magyp_spider"
    start_urls = [
        'https://www.magyp.gob.ar/sitio/areas/ss_mercados_agropecuarios/_transporte-emb/aceites.php?accion=imp'
    ]

    # Definir meses y años como atributos de clase
    meses = ["ene", "feb", "mar", "abr", "may", "jun", "jul", "ago", "sep", "oct", "nov", "dic"]
    años = ["18","19","20","21","22","23", "24"]

    def generar_links(self):
        links = []
        base = "https://www.magyp.gob.ar/sitio/areas/ss_mercados_agropecuarios/_transporte-emb/{}_{}/01_ac_pto_mes_{}_{}.php"
        for año in self.años:
            for mes in self.meses:
                link = base.format(mes, año, mes, año)
                links.append(link)
        return links

    def parse(self, response):
        links = self.generar_links()
        for link in links:
            yield response.follow(link, self.parse_month)

    def parse_month(self, response):
        # Guardar el HTML de la página para revisar
        page_content = response.text
        with open('page_content.html', 'w', encoding='utf-8') as f:
            f.write(page_content)
        
        # Extraer el dato específico usando XPath
        dato_especifico = response.xpath('/html/body/table/tbody/tr[last()]/th[position() = last() - 1]/text()').get()
        
        if dato_especifico is None:
            dato_especifico = response.xpath('/html/body/table/tbody/tr[last()]/th[position() = last() - 1]//text()').get()
        
        if dato_especifico is None:
            dato_especifico = response.xpath('//table[@class="tabla"]//tbody//tr[last()]//th[position() = last() - 1]//text()').get()
        
        if dato_especifico is None:
            dato_especifico = response.xpath('//tr[last()]//th[position() = last() - 1]//text()').get() 
        
        print("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA")    
        print(dato_especifico)    

        self.logger.info(f"URL: {response.url}")
        self.logger.info(f"Dato específico: {dato_especifico}")

        yield {
            'url': response.url,
            'dato_especifico': dato_especifico
        }



